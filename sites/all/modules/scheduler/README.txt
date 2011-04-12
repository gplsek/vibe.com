// $Id: README.txt,v 1.7 2008/02/12 00:24:23 karpuz Exp $

README
--------------------------------------------------------------------------
This module allows nodes to be published and unpublished on specified dates.
If JSCalendar is enabled (part of the JSTools module 
<http://drupal.org/node/57285>), a popup Javascript
calendar is used to select the date and time for (un)publishing of nodes, 
otherwise it defaults to text input.


This module has been completely rewritten for Drupal 4.7 by:

Ted Serbinski <hello [at] tedserbinski.com>
  aka "m3avrck" on http://drupal.org


This module was originally written for Drupal 4.5.0 by:

Moshe Weitzman <weitzman [at] tejasa.com>
Gabor Hojtsy <goba [at] php.net>
Tom Dobes <tomdobes [at] purdue.edu>




INSTALLATION
--------------------------------------------------------------------------
1. Copy the scheduler.module to your modules directory
2. Enable module, database schemas should be setup automatically. If they
   aren't, use the following SQL:
   
   MySQL:
     CREATE TABLE scheduler (
       nid int(10) unsigned NOT NULL,
       publish_on int(11) NOT NULL default '0',
       unpublish_on int(11) NOT NULL default '0',
       timezone int(6) NOT NULL default '0',
       PRIMARY KEY (nid)
     ) /*!40100 DEFAULT CHARACTER SET utf8 */;
  
   PostgresSQL:           
     CREATE TABLE scheduler (
       nid integer NOT NULL default '0',
       publish_on integer NOT NULL default '0',
       unpublish_on integer NOT NULL default '0',
       timezone integer NOT NULL default '0',
       PRIMARY KEY (nid)
     );
     
3. Grant users the permission "schedule (un)publishing of nodes" so they can
   set when the nodes they create are to be (un)published.
   
4. Visit admin > settings > content-types and click on any node type and
   check the box "enable scheduled (un)publishing" for this node type
   
5. Repeat for all node types that you want scheduled publishing for

The scheduler will run with Drupal's cron.php, and will (un)publish nodes
timed on or before the time at which cron runs.  If you'd like finer
granularity to scheduler, but don't want to run Drupal's cron more often (due
to its taking too many cycles to run every minute, for example), you can set
up another cron job for the scheduler to run independently.  Scheduler's cron
is at /scheduler/cron; a sample crontab entry to run scheduler every minute
would look like:

* * * * * /usr/bin/wget -O - -q "http://example.com/scheduler/cron"