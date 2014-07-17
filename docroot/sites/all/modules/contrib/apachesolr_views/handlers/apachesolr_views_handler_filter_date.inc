<?php

class apachesolr_views_handler_filter_date extends views_handler_filter_date {
  function op_between($field) {
    $value_a_formatted = $this->obtain_formated_date($this->value['min'], '00:00:00');
    $value_b_formatted = $this->obtain_formated_date($this->value['max'], '23:59:59.999');
    $operator = strtoupper($this->operator);
    $this->query->add_where($this->options['group'],
                            "$this->real_field",
                            '[' . $value_a_formatted . ' TO ' . $value_b_formatted . ']',
                            $operator);
  }


  //@TODO: refactor switch
  function op_simple($field) {
    $value = intval(strtotime($this->value['value'], 0));
    $value = new DateObject($value);

    // Probably there is a better way with dateobject, but no way to find how.
    $has_time = strpos($this->value['value'], ':');

    $value_format = $value->format(FACETAPI_DATE_ISO8601);
    switch ($this->operator) {
      case '<':
      case '<=':
        if ($has_time) {
          $value = '[* TO ' . $value_format . ']';
        }
        else {
          $value = '[* TO ' . $value_format . '/DAY+1DAY]';
        }
        break;
      case '>':
      case '>=':
        if ($has_time) {
          $value = '[' . $value_format . ' TO *]';
        }
        else {
          $value = '[' . $value_format . '/DAY TO *]';
        }
        break;
      case '!=':
        if ($has_time) {
          $value = '[* TO ' . $value_format . '-1SECOND] OR [' . $value_format . '+1SECOND TO *]';
        }
        else {
          $value = '[* TO ' . $value_format . '/DAY-1DAY] OR [' . $value_format . '/DAY+1DAY TO *]';
        }
        break;
      case '=':
      default:
        if ($has_time) {
          $value = '[' . $value_format . ' TO ' . $value_format . ']';
        }
        else {
          $value = '[' . $value_format . '/DAY TO ' . $value_format . '/DAY+1DAY]';
        }
        break;
    }
    $this->query->add_where($this->options['group'], "$this->real_field", $value, $this->operator);
  }

  /**
   * Add a type selector to the value form
   */
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    unset($form['value']['type']);  //Remove offset functionality.

    // Add class to identify in jquery input fields and add jquery.ui.datepicker
    if (isset($form['value']['min'])) {
      $form['value']['max']['#attributes']['class'][] = "views_input_date";
      $form['value']['min']['#attributes']['class'][] = "views_input_date";
    }
    else {
      $form['value']['value']['#attributes']['class'][] = "views_input_date";
    }
  }

  /**
   * Overload to allow ranges without end-date, asume * if not present.
   *
   * @param $input
   * @return bool
   */
  function accept_exposed_input($input) {
    if (empty($this->options['exposed'])) {
      return TRUE;
    }

    // Store this because it will get overwritten.
    $type = $this->value['type'];
    $rc = parent::accept_exposed_input($input);

    $operators = $this->operators();
    if (!empty($this->options['expose']['use_operator']) && !empty($this->options['expose']['operator_id'])) {
      $operator = $input[$this->options['expose']['operator_id']];
    }
    else {
      $operator = $this->operator;
    }

    if ($operators[$operator]['values'] != 1) {
      if ($this->value['min'] == '' && $this->value['max'] == '') {  //this is the line overrided
        return FALSE;
      }
      else {
        return TRUE;
      }
    }

    // restore what got overwritten by the parent.
    $this->value['type'] = $type;
    return $rc;
  }

  /**
   * obtains a formated data for SOLR
   * @param $date_str date in string format
   * @param $round_string string to add if no time is present
   * @return string the date formated for SOLR
   */
  function obtain_formated_date($date_str, $round_string = "00:00:00") {
    //probably there is a better way with dateobject, but no way to find how
    if ($date_str!='' && !strpos($date_str, ':')) {
      $date_str = $date_str . " " . $round_string;
    }
    $time = intval(strtotime($date_str, 0));
    if ($time != 0) {
      $date = new DateObject($time);
      $date_formatted = $date->format(FACETAPI_DATE_ISO8601);
    }
    else {
      $date_formatted = '*';
    }
    return $date_formatted;
  }
}