<?php
// $Id: twitter-pull-listing.tpl.php,v 1.1.2.5 2011/01/11 02:49:38 inadarei Exp $

/**
 * @file
 * Theme template for a list of tweets.
 *
 * Available variables in the theme include:
 *
 * 1) An array of $tweets, where each tweet object has:
 *   $tweet->id
 *   $tweet->username
 *   $tweet->userphoto
 *   $tweet->text
 *   $tweet->timestamp
 *
 * 2) $twitkey string containing initial keyword.
 *
 * 3) $title
 *
 */
?>
<div class="tweets-pulled-listing">

  <?php if (!empty($title)): ?>
    <h2><?php print $title; ?></h2>
  <?php endif; ?>

  <?php if (is_array($tweets)): ?>
    <?php $tweet_count = count($tweets); ?>
    
    <ul class="tweets-pulled-listing" style="margin-left:0;">
    <?php foreach ($tweets as $tweet_key => $tweet): ?>
      <li>
        <div class="tweet">
		<span class="tweet-text"><?php print twitter_pull_add_links($tweet->text); ?></span>
		<div class="tweet-time"><?php print l($tweet->time_ago, 'http://twitter.com/' . $tweet->username . '/status/' . $tweet->id);?></div>
		</div>
		
		
        <?php if ($tweet_key < $tweet_count - 1): ?>
          <div class="tweet-hr"></div>
        <?php endif; ?>
        
      </li>
    <?php endforeach; ?>
    </ul>
  <?php endif; ?>
</div>
