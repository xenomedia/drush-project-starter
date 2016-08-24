# drush-project-starter for Drupal 7

## Install 
download to sites/all/drush
edit:

- aliases.drush.inc
    + setup $aliases['dev']
    + setup $aliases['stage']
    + Add other sites as needed i.e. $aliases['prod']
- croncheck.drush.inc
    + change email on croncheck.drush.inc:65
    + change URL on croncheck.drush.inc:100
    + change URL on croncheck.drush.inc:104 (2 instances)
    + change URL on croncheck.drush.inc:105 (2 instances)
- create patches.make in the root of your site (or edit drushrc.php and edit the location "$options['patch-file'] = 'patches.make';") 
    + Add "api = 2"
    + Add "core = 7.x"

## Optional reading

- [Drush Patch File](https://bitbucket.org/davereid/drush-patchfile)
- [Drupal, Drush aliases and how to use them](https://www.deeson.co.uk/labs/drupal-drush-aliases-and-how-use-them)
- [(go-local) This custom drush command will make you a better Drupal developer](https://www.thirdandgrove.com/this-custom-drush-command-will-make-you-a-better-drupal-developer)
- [Drush](http://www.drush.org/en/master/)
