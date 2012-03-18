<?php
/**
 * @file
 * upload.admin.inc
 *
 */

/**
 * Form API callback to validate the upload settings form.
 */
function islandora_upload_admin_settings_validate($form, &$form_state) {
  $default_uploadsize = $form_state['values']['islandoraupload_uploadsize_default'];
  $exceed_max_msg = t('Your PHP settings limit the maximum file size per upload to %size.', array('%size' => format_size(file_upload_max_size()))) .'<br/>';
  $more_info = t("Depending on your server environment, these settings may be changed in the system-wide php.ini file, a php.ini file in your Drupal root directory, in your Drupal site's settings.php file, or in the .htaccess file in your Drupal root directory.");

  if (!is_numeric($default_uploadsize) || ($default_uploadsize <= 0)) {
    form_set_error('islandoraupload_uploadsize_default', t('The %role file size limit must be a number and greater than zero.', array('%role' => t('default'))));
  }

  if ($default_uploadsize * 1024 * 1024 > file_upload_max_size()) {
    form_set_error('islandoraupload_uploadsize_default', $exceed_max_msg . $more_info);
    $more_info = '';
  }

  foreach ($form_state['values']['roles'] as $rid => $role) {
    $uploadsize = $form_state['values']['islandoraupload_uploadsize_'. $rid];

    if (!is_numeric($uploadsize) || ($uploadsize <= 0)) {
      form_set_error('islandoraupload_uploadsize_'. $rid, t('The %role file size limit must be a number and greater than zero.', array('%role' => $role)));
    }
    if ($uploadsize * 1024 * 1024 > file_upload_max_size()) {
      form_set_error('islandoraupload_uploadsize_'. $rid, $exceed_max_msg . $more_info);
      $more_info = '';
    }
  }
}

/**
 * Menu callback for the upload settings form.
 */
function islandora_upload_admin_settings() {
  $islandoraislandoraupload_extensions_default = variable_get('islandoraislandoraupload_extensions_default', '3dm 3dmf a aab aam aas abc acgi afl ai aif aifc aiff aim aip ani aos aps arc arj art asf asm asp asx au avi avs bcpio bin bm bmp boo book boz bsh bz bz2 c c++ cat cc ccad cco cdf cer cha chat class com conf cpio cpp cpt crl crt csh csh css cxx dcr deepv def der dif dir dl doc dot dp drw dump dv dvi dwf dwg dxf dxr el elc env eps es etx evy exe f f77 f90 fdf fif fli flo flx fmf for fpx frl funk g g3 gif gl gsd gsm gsp gss gtar gz gzip h hdf help hgl hh hlb hlp hpg hpgl hqx hta htc htm html htmls htt htx ice ico idc ief iefs iges igs ima imap inf ins ip isu it iv ivr ivy jam jav java jcm jfif jfif-tbnl jpe jpeg jpg jps js jut kar ksh la lam latex lha lhx list lma log lsp lst lsx ltx lzh lzx m m1v m2a m2v m3u man map mar mbd mc$ mcd mcf mcp me mht mhtml mid midi mif mime mjf mjpg mm mme mod moov mov movie mp2 mp3 mpa mpc mpe mpeg mpg mpga mpp mpt mpv mpx mrc ms mv my mzz nap naplps nc ncm nif niff nix nsc nvd o oda omc omcd omcr p p10 p12 p7a p7c p7m p7r p7s part pas pbm pcl pct pcx pdb pdf pfunk pgm pic pict pkg pko pl plx pm pm4 pm5 png pnm pot pov ppa ppm pps ppt ppz pre prt ps psd pvu pwz py pyc qcp qd3 qd3d qif qt qtc qti qtif ra ram ras rast rexx rf rgb rm rmi rmm rmp rng rnx roff rp rpm rt rtf rtx rv s s3m saveme sbk scm sdml sdp sdr sea set sgm sgml sh shar shtml sid sit skd skm skp skt sl smi smil snd sol spc spl spr sprite src ssi ssm sst step stl stp sv4cpio sv4crc svf svr swf t talk tar tbk tcl tcsh tex texi texinfo text tgz tif tif tiff tr tsi tsp tsv turbot txt uil uni unis unv uri uris ustar uu uue vcd vcs vda vdo vew viv vivo vmd vmf voc voc vos vox vqe vqf vql vrml vrt vsd vst vsw w60 w61 w6w wav wb1 wbmp web wiz wk1 wmf wml wmlc wmls wmlsc word wp wp5 wp6 wpd wq1 wri wrl wrz wsc wsrc wtk xbm xdr xgz xif xl xla xlb xlc xld xlk xll xlm xls xlt xlv xlv xlw xm xml xmz xpix xpm xpm x-png xsr xwd xwd xyz z zip zip zip zip zoo zsh');
  $islandoraupload_uploadsize_default = variable_get('islandoraupload_uploadsize_default', 1);

  $form['settings_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
  );

  $form['settings_general']['islandoraislandoraupload_extensions_default'] = array(
    '#type' => 'textarea',
    '#title' => t('Default permitted file extensions'),
    '#default_value' => $islandoraislandoraupload_extensions_default,
    '#maxlength' => 2000,
    '#description' => t('Default extensions that users can upload.'),
    '#cols' => 15,
    '#rows' => 15,
    '#attributes' => array('readonly' => 'readonly'),
  );
  $form['settings_general']['islandoraupload_uploadsize_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Default maximum file size per upload'),
    '#default_value' => $islandoraupload_uploadsize_default,
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('The default maximum file size a user can upload.'),
    '#field_suffix' => t('MB'),
  );

  $form['settings_general']['upload_max_size'] = array('#value' => '<p>'. t('Your PHP settings limit the maximum file size per upload to %size.', array('%size' => format_size(file_upload_max_size()))) .'</p>');

  $roles = user_roles(FALSE, 'attach files');
  $form['roles'] = array('#type' => 'value', '#value' => $roles);

  foreach ($roles as $rid => $role) {
    $form['settings_role_'. $rid] = array(
      '#type' => 'fieldset',
      '#title' => t('Settings for @role', array('@role' => $role)),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['settings_role_'. $rid]['islandoraupload_uploadsize_'. $rid] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum file size per upload'),
      '#default_value' => variable_get('islandoraupload_uploadsize_'. $rid, $islandoraupload_uploadsize_default),
      '#size' => 5,
      '#maxlength' => 5,
      '#description' => t('The maximum size of a file a user with the ' . $role . ' role can upload.'),
      '#field_suffix' => t('MB'),
    );
  }

  $form['#validate'] = array('islandora_upload_admin_settings_validate');

  return system_settings_form($form);
}
