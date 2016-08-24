<?php

/**
 * @file
 *   Drush command to check on cron and make sure it has been run.
 *
 * @author
 *   Dave Look http://drupal.org/user/237476
 */

/**
 * Implements hook_drush_command().
 */
function croncheck_drush_command() {
  $items = array();

  $items['checkcron'] = array(
    'description' => "Check on time since cron run and send alert if needed.",
    'aliases'     => array('checkc'),
    'bootstrap'   => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}


/**
 * Implements hook_drush_help().
 */
function croncheck_drush_help($section) {
  switch ($section) {
    case 'drush:checkcron':
      return dt("This command will check to see the last time cron ran and send an alert if needed.");

    case 'meta:checkcron:title':
      return dt("checkcron commands");

    case 'meta:checkcron:summary':
      return dt("Keeping cron running helps to keep the site running smoothly.");
  }
}

/**
 * Main drush command callback. This is where the action takes place.
 */
function drush_croncheck_checkcron() {

  // Get the last time cron was run via the cron_last variable.
  $last_cron = variable_get('cron_last', '');
  // Make the default time to check one hour (3600s). Use the
  // drushcroncheck_time_to_check variable to change this.
  $time_to_check = '3600';
  // Get the difference in seconds between now and the last time cron ran.
  $difference = time() - $last_cron;

  // Has it been longer than our time to check since cron ran?
  if ($difference >= $time_to_check) {
    // Format the date so it is more readable than timestamp seconds.
    $cleandate = date('l F d Y - G:i:s A', $last_cron);
    // Format a message to be used via the command line OR the email.
    $message = check_plain('We have a problem, ' . $cleandate . ' is the last time cron ran! Time since last cron is: ' . drush_timesince($difference));
    // Uncomment this line if you want to have the display in the command line.
    // drush_print($message);
    // Configure and send the email.
    $recipients = 'user@example.com';
    drush_mail_send_email($recipients, 'example.com: Cron has failed or hung', $message);

  }
  else {
    // Add whatever is needed for successful cron runs, this is unneeded now.
    drush_print('All good');
  }
}


/**
 * Send an e-mail to a specified e-mail address.
 *
 * See http://drupal.org/files/mail.drush_.inc__0.txt.
 */
function drush_mail_send_email($to, $subject, $body) {
  // Define $from and headers.
  if (!$from = drush_get_option('from')) {
    $from = variable_get('site_mail', ini_get('sendmail_from'));
  }
  $headers = array();
  $headers['From'] = $headers['Sender'] = $headers['Return-Path'] = $headers['Errors-To'] = $from;
  $headers['X-Priority'] = 1;
  $headers['X-MSMail-Priority'] = 'High';
  $headers['Importance'] = 'High';

  // D7 implementation of drupal_mail.
  if (function_exists('drupal_mail_system')) {
    // Prepare the message.
    $message = drupal_mail('drush', 'key', $to, NULL, array(), $from, FALSE);

    $message['subject'] = $subject;
    $message['body'] = array();
    $message['body'][] = '<p>' . $body . '</p>';
    $message['body'][] = '<p>Location - Live server at http://www.example.com/</p>';
    $message['body'][] = '<ul>Some steps to finding and correcting the issue
        <li>Check the error logs for PHP or MySQL errors</li>
        <li>Check access logs to determine if the server experiencing unusual or high traffic. High traffic while running cron can cause failures if the sever runs out of resources.</li>
        <li>Try running cron manually <a href="http://www.example.com/admin/config/system/cron">http://www.example.com/admin/config/system/cron</a></li>
        <li>If cron won\'t run manually delete the cron semaphore in the database. Go to <a href="http://www.example.com/devel/variable">http://www.example.com/devel/variable</a> and delete the variables titled "cron_last" and "cron_semaphore" then clear drupal cache and try to run cron again. <a href="https://www.drupal.org/node/553430">More information here</a></li>
        <li>Try restarting apache { $ sudo service spache2 restart } </li>
        <li>Try restarting mysql {  $ sudo service mysql restart } </li>
        <li>Try restarting the server { $ sudo reboot } </li>
</ul>';
    $message['headers'] = $headers;

    // Retrieve the responsible implementation for this message.
    $system = drupal_mail_system('drush', 'key');
    // Format the message body.
    $message = $system->format($message);
    // Send e-mail.
    $message['result'] = $system->mail($message);
    $result = $message['result'];

    // D6 implementation of drupal_mail_send.
  }
  else {
    $message = array(
      'to'      => $to,
      'subject' => $subject,
      'body'    => $body,
      'headers' => $headers,
    );
    $result = drupal_mail_send($message);
  }

  // Return result.
  if ($result) {
    drush_log(dt('E-mail message sent to <!to>', array('!to' => $to)), 'ok');
  }
  else {
    drush_set_error('DRUSH_MAIL_ERROR', dt('An error occurred while sending the e-mail message.'));
  }
}

/**
 * Helper function to transfor seconds into a usable time since string
 * @see http://www.neowin.net/forum/topic/806866-changing-seconds-into-days-hours-minutes-seconds/page__p__591387898#entry591387898
 */

function drush_timesince($seconds) {

  if ($seconds > 3600) {
    $days = $seconds / 86400;
    $day_explode = explode(".", $days);
    $day = $day_explode[0];
  }
  else {
    $day = 0;
  }

  if ($seconds > 3600) {
    $hours = '.' . @$day_explode[1] . '';
    $hour = $hours * 24;
    $hourr = explode(".", $hour);
    $hourrs = $hourr[0];
  }
  else {
    $hours = $seconds / 3600;
    $hourr = explode(".", $hours);
    $hourrs = $hourr[0];
  }

  $minute = '.' . @$hourr[1] . '';
  $minutes = $minute * 60;
  $minute = explode(".", $minutes);
  $minuter = $minute[0];

  $seconds = '.' . @$minute[1] . '';
  $second = $seconds * 60;
  $second = round($second);

  return (@$day . ' Days ' . @$hourrs . ' Hours ' . @$minuter . ' minutes, ' . @$second . ' seconds');
}
