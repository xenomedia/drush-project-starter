<?php
$aliases['dev'] = array (
  'root' => '~/Sites/example/www',
  'uri' => 'http://example.dev',
  'path-aliases' => array(
    '%dump-dir' => '/tmp',
    '%files' => 'sites/default/files',
  ),
);

$aliases['stage'] = array(
  'uri' => 'http://www.example.com/',
  'root' => '/var/www/html/',
  'remote-host' => 'ssh.example.com',
  'remote-user' => 'ssh_user',
  'path-aliases' => array(
    '%dump-dir' => '/tmp',
    '%files' => 'sites/default/files',
  ),
);
