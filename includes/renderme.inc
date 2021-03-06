<?php
/*
 *  Copyright (c) 2009 Denic Wibowo<denicwibowo@gmail.com>.
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 *  Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *  renderme.tpl.php
 *
 *  Created on Sep 6, 2009, 11:03:48 AM
 */
//echo '<pre>'; print_r($content); echo '</pre>';

require_once THEMES_DIR . DS . $theme . DS . 'config.inc';
if(isset($content['output']['form'])){
//        echo 'i am here !';
    $formlists = array();
    foreach($content['output']['form'] as $value){
        foreach($value as $key=>$val){
            $formlists[] = $key;
            ${$key} = $val;
        }
    }
    if(in_array('auth_login_form', $content['exec'])){
        $userkey = '';
        foreach($login as $key=>$value){
            if(!eregi('#', $key)){
                $userkey = $key; break;
            }
        }
        $body_login = "javascript:document." . $formlists[0] . "." . $userkey . ".focus()";
    }
}

$itemselement = array(
    'type',
    'title',
    'collapsible',
    'collapsed',
    'weight',
    'action',
    'message',
    'error'
);

//echo '<pre>'; print_r($content['output']['form']); echo '</pre>';

function __create_form_special($data){
    global $forminput, $formtextarea, $formselect;
    $result = '';
    foreach($data as $key=>$value){
        $formtpl = $value['#formtpl']; break;
    }
    require_once $formtpl; unset($formtpl);
    return $result;
}

function __create_items_detail($value){
    global $forminput, $formtextarea, $formselect, $user;
    $result = '';
    if(isset($value['title']) && $value['title'] != 'hidden'){
        $result .= '<div>'. ucwords($value['title']) .'</div>' . "\n";
    }
    foreach($value['data'] as $key => $val){
        if(isset($val['form'])){
            if(in_array($val['form']['detail'][$key]['#type'], $forminput) || in_array($val['form']['detail'][$key]['#type'], $forminput)){
                $result .= '<body onload="javascript:document.detail.' . $key . '.focus()">';
                break;
            }
        }
    }
    $result .= '<div id="form">' . "\n";
    foreach($value['data'] as $key => $val){
        if ($val['hide'] != '1'){
            if(!isset($value['update_access'])){$ok = 1;}
            elseif($value['update_access'] == $user['groups'] || $user['groups'] == 'admin'){$ok = 1;}
            elseif(__get_group_permission($value['update_access']) !== FALSE){$ok = 1;}
            $result .= '<label for="' . $key . '" style="font-weight:bold">' . ucwords($val['key']) . ' :</label>' . "\n";
            $result .= '<div>';
            if(isset($val['form'])){
                if(isset($ok)){
                    $result .= __create_form($val['form']);
                }
            } else {
                if (isset($val['link']) && isset($ok)){
                    $result .= '<a href="' . $val['link'] . '">';
                }
                $result .= $val['value']!='' ? ucwords($val['value']) : ' &nbsp;&nbsp;&nbsp; ';
                if(isset($val['link']) && isset($ok)){
                    $result .= '</a>';
                }
            }
            $result .= '</div>' . "\n";
            unset($ok);
        }
    }
    $result .= '</div>'. "\n";
    if($value['dropable'] != '0'){
        if(!isset($value['update_access'])){$ok = 1;}
        elseif($value['update_access'] == $user['groups'] || $user['groups'] == 'admin'){$ok = 1;}
        elseif(__get_group_permission($value['update_access']) !== FALSE){$ok = 1;}
        if(isset($ok)){
            unset($ok);
            $result .= '<div class="error" style="text-align:center;right:20%">';
            $valdrop[$value['id']] = array(
                '#type' => 'fieldset',
                '#title' => 'hidden',
                '#action' => $value['dropable'],
                'id' => array(
                    '#type' => 'hidden',
                    '#value' => $value['id']
                ),
                'submit' => array(
                    '#type' => 'submit',
                    '#value' => __t('drop'),
                    '#style' => 'background:red;color:white'
                )
            );
            $result .= __create_form($valdrop);
            $result .= '</div>';
        }
    }
    return $result;
}

function __create_table($value){
//    echo '<pre>'; print_r($value); echo '</pre>';
    global $user;
    $result = '';
    if(isset($value['addmenu'])){
        if(!isset($value['addmenu']['user_required'])){$ok = 1;}
        elseif($value['addmenu']['user_required'] == $user['groups'] || $user['groups'] == 'admin'){$ok = 1;}
        elseif(__get_group_permission($value['addmenu']['user_required']) !== FALSE){$ok = 1;}
        if(isset($ok)){
            unset($ok);
            $result .= '<div id="sticky-button">';
            $valadd['add'] = array(
                '#type' => 'fieldset',
                '#title' => 'hidden',
                '#action' => $value['addmenu']['action'],
                'submit' => array(
                    '#type' => 'submit',
                    '#value' => $value['addmenu']['title'],
                    '#class' => 'button'
                )
            );
            $result .= __create_form($valadd);
            $result .= '</div>';
        }
    }
    $result .= "<!--div style=\"font-wieght:bold;font-size:1em\">" . strtoupper($value['title']) . "</div-->\n";
    $result .= '<div>' . strtoupper($value['title']) . '</div>' . "\n";
    $result .= '<table width="100%" class="tbl">' . "\n";
    foreach($value['header'] as $key=>$val){
        $result .= '<th' . (isset($val['width']) ? ' width="' . $val['width'] . '"' : '') . '>' . ucwords($val['caption']) . '</th>' . "\n";
    }
    if(count($value['data']) > 0){
        foreach($value['data'] as $key => $val){
            $result .= '<tr class="menu_plain" onmouseover=\'this.className="menu_highlight"\' onmouseout=\'this.className="menu_plain"\' style="cursor:pointer" onclick=\'location.href="' . $val['goto'] . '"\'>' . "\n";
            foreach($value['header'] as $ky=>$vl){
                $result .= '<td align="' . $vl['align'] . '">' . "\n";
                if($vl['field'] == 'none'){
                    $result .= $key+1;
                } else {
                    $result .= $val[$vl['field']];
                }
                $result .= '</td>' . "\n";
            }
            $result .= '</tr>' . "\n";
        }
    }
    $result .= '</table>';
    return $result;
}

function __create_form_items($data, $iditem=NULL, $classitem=NULL){
    global $forminput, $formtextarea, $formselect;
    $result = '';
//    echo '<pre>'; print_r($data); echo '</pre>';
    foreach($data as $key => $value){
        $result .= '<div>';
        if(isset($value['#title']) && $value['#title'] != 'hidden' && $value['#type'] != 'hidden'){
            $result .= '<label for="'. $key .'">' . ucwords($value['#title']) . '</label>';
        }
        if(in_array($value['#type'], $forminput)){
            if($value['#type'] == 'checkbox' || $value['#type'] == 'radio'){
                $result .= '<div>' . "\n"; $o = 0;
                foreach($value['#option'] as $ky => $vl){
                    $namekey = $value['#type'] == 'checkbox' ? $key . '[' . $o . ']' : $key;
                    if($o > 0){
                        $result .= '<label for="'. $namekey .'">&nbsp;</label>';
                    }
                    $result .= '<div>';
                    $result .= '<input type="' . $value['#type'] . '" name="' . $namekey . '" value="' . $ky . '"';
                    if(in_array($ky, $value['#value'])){
                        $result .= ' checked="true"';
                    }
                    if(isset($value['#style'])){
                        $result .= ' style="' . $value['#style'] . '"';
                    }
                    if(isset($value['#extra'])){
                        $result .= ' ' . $value['#extra'];
                    }
                    if(isset($iditem) && !is_null($iditem)){
                        $result .= ' id="' . $iditem . '"';
                    }
                    if(isset($classitem) && !is_null($classitem)){
                        $result .= ' class="' . $classitem . '"';
                    }
                    $result .= '>';
                    $result .= ' ' . $vl;
                    $result .= '</div>' . "\n";
                    $o++;
                    unset($namekey);
                }
                $result .= '</div>' . "\n";
            } else {
                $result .= '<input type="' . $value['#type'] . '" name="' . $key . '"';
                if(isset($value['#size'])){
                    $result .= ' size="' . $value['#size'] . '"';
                }
                if(isset($value['#value'])){
                    $result .= ' value="' . ($value['#type'] == 'submit' ? ucwords($value['#value']) : $value['#value']) . '"';
                }
                if(isset($value['#style'])){
                    $result .= ' style="' . $value['#style'] . '"';
                }
                if(isset($value['#extra'])){
                    $result .= ' ' . $value['#extra'];
                }
                if(isset($iditem) && !is_null($iditem)){
                    $result .= ' id="' . $iditem . '"';
                }
                if(isset($classitem) && !is_null($classitem)){
                    $result .= ' class="' . $classitem . '"';
                }
                if($value['#id'] == 'login' && $value['#type'] == 'submit'){
                    $result .= ' class="button"';
                }
                $result .= '>' . "\n";
            }
        } elseif(in_array($value['#type'], $formtextarea)){
            $result .= '<textarea name="' . $key . '" cols="' . (isset($value['#cols']) ? $value['#cols'] : '40') . '" rows="' . (isset($value['#rows']) ? $value['#rows'] : '3') . '"';
            if(isset($value['#style'])){
                $result .= ' style="' . $value['#style'] . '"';
            }
            if(isset($value['#extra'])){
                $result .= ' ' . $value['#extra'];
            }
            if(isset($iditem) && !is_null($iditem)){
                $result .= ' id="' . $iditem . '"';
            }
            if(isset($classitem) && !is_null($classitem)){
                $result .= ' class="' . $classitem . '"';
            }
            $result .= '>';
            if(isset($value['#value'])){
                $result .= $value['#value'];
            }
            $result .= '</textarea>' . "\n";
        } elseif(in_array($value['#type'], $formselect)){
            $result .= '<select name="' . $key . '" size="' . (isset($value['#size']) ? $value['#size'] : 1) . '"';
            if(isset($value['#style'])){
                $result .= ' style="' . $value['#style'] . '"';
            }
            if(isset($value['#extra'])){
                $result .= ' ' . $value['#extra'];
            }
            if(isset($iditem) && !is_null($iditem)){
                $result .= ' id="' . $iditem . '"';
            }
            if(isset($classitem) && !is_null($classitem)){
                $result .= ' class="' . $classitem . '"';
            }
            $result .= '>';
            if(isset($value['#option_blank']) && $value['#option_blank'] == '1'){
                $result .= '<option value=""></option>' . "\n";
            }
            foreach($value['#option'] as $yk => $lv){
                $result .= '<option value="' . $yk . '"';
                if($yk == $value['#value']){
                    $result .= ' selected';
                }
                $result .= '>' . $lv . '</option>';
            }
            $result .= '</select>';
        } elseif($value['#type'] == 'date'){
            $value['#value'] = $value['#value'] == '' ? array('1970', '01', '01') : explode('-', $value['#value']);
            krsort($value['#value']);
            foreach($value['#value'] as $kk => $vv){
                if($kk == 1){
                    $result .= '<select name="' . $key . '[' . $kk . ']" size="1"';
                    if(isset($value['#style'])){
                        $result .= ' style="' . $value['#style'] . '"';
                    }
                    if(isset($value['#extra'])){
                        $result .= ' ' . $value['#extra'];
                    }
                    if(isset($iditem) && !is_null($iditem)){
                        $result .= ' id="' . $iditem . '"';
                    }
                    if(isset($classitem) && !is_null($classitem)){
                        $result .= ' class="' . $classitem . '"';
                    }
                    $result .= '>';
                    foreach($value['#option'] as $yk => $lv){
                        $result .= '<option value="' . $yk . '"';
                        if($yk == $vv){
                            $result .= ' selected';
                        }
                        $result .= '>' . ucwords($lv) . '</option>';
                    }
                    $result .= '</select>' . "\n";
                } else {
                    $result .= '<input type="text" name="' . $key . '[' . $kk . ']" maxlength="' . ($kk > 0 ? 2 : 4) . '" size="' . ($kk > 0 ? 2 : 4) . '"';
                    if(isset($value['#value'])){
                        $result .= ' value="' . $vv . '"';
                    }
                    if(isset($value['#style'])){
                        $result .= ' style="' . $value['#style'] . '"';
                    }
                    if(isset($value['#extra'])){
                        $result .= ' ' . $value['#extra'];
                    }
                    if(isset($iditem) && !is_null($iditem)){
                        $result .= ' id="' . $iditem . '"';
                    }
                    if(isset($classitem) && !is_null($classitem)){
                        $result .= ' class="' . $classitem . '"';
                    }
                    if($value['#id'] == 'login' && $value['#type'] == 'submit'){
                        $result .= ' class="button"';
                    }
                    $result .= '>' . "\n";
                }
            }
        } elseif($value['#type'] == 'info'){
            $result .= ucwords(strtolower($value['#value']));
        }
        if(isset($value['#message'])){
            $result .= ' ' . $value['#message'];
        }
        $result .= '</div>' . "\n";
    }
    return $result;
}

function __create_form_open($data, $name, $idform=NULL, $classform=NULL){
    $result = '';
    if(isset($data['#focused_item'])){
        $result .= '<body onload="javascript:document.' . $name . '.' . $data['#focused_item'] . '.focus()">';
    }
    if(isset($data['#error']) && $data['#error'] !== FALSE){
        $result .= '<div class="error">' . $data['#error'] . '</div><br /><br />' . "\n";
    }
    $result .= '<form action="' . $data['#action'] . '" method="POST" name="' . $name . '"';
    if(isset($idform) && !is_null($idform)){
        $result .= ' id="' . $idform . '"';
    }
    if(isset($classform) && !is_null($classform)){
        $result .= ' class="' . $classform . '"';
    }
    $result .= '>' . "\n";
    if(isset($data['#title']) && $data['#title'] != 'hidden'){
        $result .= '<div>'. ucwords($data['#title']) .'</div>' . "\n";
    }
    if(isset($data['#message']) && $data['#message'] != 'hidden'){
        if(is_array($data['#message'])){
            foreach($data['#message'] as $ky => $vl){
                $result .= '<div>';
                if($data['#id'] == 'login' && $ky < 1){
                    $result .= '<font class="login-title">';
                }
                $result .= ucwords($vl);
                if($data['#id'] == 'login'){
                    if($ky < 1){
                        $result .= '</font>';
                    } else {
                        $result .= '<br /><br />';
                    }
                }
                $result .= '</div>' . "\n";
            }
        } else {
            $result .= '<div>'. ucwords($data['#message']) .'</div>' . "\n";
        }
    }
    return $result;
}

function __create_form_close(){
    $result = '';
    $result .= '</form>' . "\n";
    return $result;
}

function __create_form($dataform, $idform=NULL, $classform=NULL, $iditem=NULL, $classitem=NULL){
    global $forminput, $formtextarea, $formselect;
    $result = '';
    foreach($dataform as $key => $value){
        $created = array(); $formhead = array();
        foreach($value as $ky=>$vl){
            if(!eregi('#', $ky)){
                $created[$ky] = $vl;
            } else {
                $formhead[$ky] = $vl;
            }
        }
        $result .= __create_form_open($formhead, $key, $idform, $classform); unset($formhead);
        $result .= __create_form_items($created, $iditem, $classitem); unset($created);
        $result .= __create_form_close();
    } unset($created);
    return $result;
}

function __create_info($value){
    $result = '';
    $result .= '<div>';
    $result .= $value['show'];
    $result .= '</div>' . "\n";
    return $result;
}

require_once THEMES_DIR . DS . $theme . DS . 'page.tpl';

//end of file;
