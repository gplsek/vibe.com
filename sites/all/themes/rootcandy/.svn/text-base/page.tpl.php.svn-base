<?php
// $Id: page.tpl.php,v 1.23.2.16 2009/06/20 16:53:25 sign Exp $

?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>">
  <head>
    <title><?php print $head_title ?></title>
    <?php print $head ?>
    <?php print $styles ?>
    <?php print $scripts ?>
  </head>
  <?php
  // get admin links into the region
  if (arg(0) == 'admin' AND arg(2)) {
    $menu = menu_navigation_links('navigation', 2);
    $menu_links = _rootcandy_links($menu, array('id' => 'rootcandy-menu'));
    if ($language->direction) {
      $admin_right = $menu_links . $admin_right;
    }
    else {
      $admin_left = $menu_links . $admin_left;
    }
  }
  ?>
  <body<?php print rootcandy_body_class($admin_left, $admin_right); ?>>

    <!-- Layout -->
    <?php if (!$hide_header) { ?>
    <div id="toppanel">
      <div id="panel">
        <?php print $slider ?>
      </div> <!-- /login -->
      <div id="toppanel-head">
        <div id="go-home">
          <?php if (isset($go_home)) print $go_home; ?>
        </div>
        <div id="admin-links">
          <?php print _rootcandy_admin_links() ?>
        </div>
        <?php if (!$hide_panel) { ?>
        <div id="header-title" class="clearfix">
          <ul id="toggle"><li><?php print $panel_navigation ?></li></ul>
        </div>
        <?php } ?>
      </div>
    </div>
    <?php } ?>
    <div id="page-wrapper"><div id="page-wrapper-content">
      <?php print $header ?>
      <div id="navigation" <?php if (!$hide_header) print 'class="header-on"' ?>>
        <?php print _rootcandy_admin_navigation() ?>

      <?php
      if ($logo) {
        print '<img src="'. check_url($logo) .'" alt="'. $site_name .'" id="logo" />';
      }
      ?>
      </div>

      <div id="breadcrumb" class="alone">
        <?php if ($title): print '<h2 id="title">'. $title .'</h2>'; endif; ?>
        <?php print $breadcrumb; ?>
      </div>

      <div id="content-wrap">
        <div id="inside">
          <?php if ((arg(0) == 'admin' AND (arg(2))) || $admin_left) { ?>
            <div id ="sidebar-left">
              <?php
              print $admin_left;
              ?>
            </div>
          <?php } ?>
          <?php if ($admin_right) { ?>
            <div id ="sidebar-right">
              <?php
              print $admin_right;
              ?>
            </div>
          <?php } ?>
          <div id="content">
            <div class="t"><div class="b"><div class="l"><div class="r"><div class="bl"><div class="br"><div class="tl"><div class="tr"><div class="content-in">
              <?php
                // TODO
                // $menu_sublinks = menu_navigation_links('navigation', 3);
                // if ($menu_sublinks) {
                //  print theme('links', $menu_sublinks, array('id' => 'rootcandy-menu'));
                // }
              ?>
              <?php if (isset($tabs) && $tabs): print '<div id="tabs-primary"><ul class="tabs primary">'. $tabs .'</ul></div><div class="level-1">'; endif; ?>
              <?php if (isset($tabs2) && $tabs2): print '<div id="tabs-secondary"><ul class="tabs secondary">'. $tabs2 .'</ul></div><div class="level-2">'; endif; ?>
              <?php
                //dashboard
                if (isset($dashboard)) {
              ?>
                <div id="dashboard" class="clearfix">
                  <div id="dashboard-left">
                    <?php print $dashboard_left ?>
                  </div>
                  <div id="dashboard-right">
                    <?php print $dashboard_right ?>
                  </div>
                </div>
              <?php
                }
                else {
                  print $help;
                  print $messages;
                }
              ?>
              <?php
                if (!$hide_content) {
                  print $content;
                }
              ?>
              <?php if (isset($tabs2) && $tabs2): print '</div>'; endif; ?>
              <?php if (isset($tabs) && $tabs): print '</div>'; endif; ?>
            </div><br class="clear" /></div></div></div></div></div></div></div></div>
          </div>
        </div>
      </div>
      <?php if ($footer) { ?>
        <div id="footer">
          <?php print $footer; ?>
        </div>
      <?php } ?>
    </div></div>
    <!-- /layout -->
    <?php print $closure ?>
  </body>
</html>