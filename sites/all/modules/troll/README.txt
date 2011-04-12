********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: troll module
Authors: Aaron Welch <welch at advomatic dot com>
         David Kent Norman http://deekayen.net/
	 Jeff Warrington <jeff at qoolio dot org>
Dependencies:
  for host lookups, the exec() command must be available in php,
  and the 'host' command must be available on the shell

********************************************************************
DESCRIPTION:

troll.module provides troll management tools for community sites including:
- user tracking by ip address
- bans IP addresses forever or for set duration, by ban I mean redirect, and by redirect I mean complete site blocking not just account creation and login
- advanced user account searching
- easy user blocking by role
- punish visitors listed in blacklists such as SPEWS.org and okean.com. Note IP banning/blocking differs from blacklisting.

********************************************************************
INSTALLATION:

see the INSTALL file in this directory

********************************************************************
WISH LIST:

- count hits from trolls (maybe just with variable_get/set()?)
- gototrollshouseandconfinscatethiercomputer.module plugin
