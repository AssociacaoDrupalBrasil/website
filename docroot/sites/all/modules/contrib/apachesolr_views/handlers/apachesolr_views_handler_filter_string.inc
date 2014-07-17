<?php

class apachesolr_views_handler_filter_string extends views_handler_filter_string {
  function operators() {
    $operators = parent::operators();
    foreach (array('shorterthan', 'longerthan', 'regular_expression',
        'contains', 'ends', 'not_ends') as $key) {
      if (isset($operators[$key])) {
        unset($operators[$key]);
      }
    }
    return $operators;
  }

  function op_equal($field) {
    if ($this->operator == '!=') {
      $this->real_field = '-' . $this->real_field;
    }
    $this->query->add_where($this->options['group'], $this->real_field, $this->value);
  }

  function op_word($field) {
    $where_operator = $this->operator == 'word' ? ' OR ' : ' AND ';
    $where = array();

    // Don't filter on empty strings.
    if (empty($this->value)) {
      return;
    }

    preg_match_all('/ (-?)("[^"]+"|[^" ]+)/i', ' ' . $this->value, $matches, PREG_SET_ORDER);
    foreach ($matches as $match) {
      $phrase = false;
      // Strip off phrase quotes
      if ($match[2]{0} == '"') {
        $match[2] = substr($match[2], 1, -1);
        $phrase = true;
      }
      $words = trim($match[2], ',?!();:-');
      $words = $phrase ? array($words) : preg_split('/ /', $words, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($words as $word) {
        $where[] = trim($word, " ,!?") . '*';
      }
    }

    if (!$where) {
      return;
    }

    // previously this was a call_user_func_array but that's unnecessary
    // as views will unpack an array that is a single arg.
    $this->query->add_where($this->options['group'], $this->real_field, implode($where_operator, $where));
  }

  function op_starts($field) {
    $this->query->add_where($this->options['group'], $this->real_field, $this->value . '*');
  }

  function op_not_starts($field) {
    $this->query->add_where($this->options['group'], '-' . $this->real_field, $this->value . '*');
  }

  function op_not($field) {
    $this->query->add_where($this->options['group'], '-' . $this->real_field, $this->value);
  }
}
