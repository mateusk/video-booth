<?php
require('tfpdf.php');

$name = $_POST['name'];
$rg = $_POST['rg'];
$phone = $_POST['phone'];

if($name == ""|| $phone == "" || $rg == ""){
    header("Location: requisitarInfo.html");
}

else{
  $pdf = new tFPDF();
  $pdf->AddPage();

  // Add a Unicode font (uses UTF-8)
  $pdf->AddFont('DejaVu','','DejaVuSansCondensed.ttf',true);
  $pdf->SetFont('DejaVu','',14);

  $month = "";

  switch (date("n")) {
    case 1:
        $month = "janeiro";
        break;
    case 2:
        $month = "fevereiro";
        break;
    case 3:
        $month = "março";
        break;
    case 4:
        $month = "abril";
        break;
    case 5:
        $month = "maio";
        break;
    case 6:
        $month = "junho";
        break;
    case 7:
        $month = "julho";
        break;
    case 8:
        $month = "agosto";
        break;
    case 9:
        $month = "setembro";
        break;
    case 10:
        $month = "outubro";
        break;
    case 11:
        $month = "novembro";
        break;
    case 12:
        $month = "dezembro";
        break;
    default:
        $month = "janeiro";
  }

  // Load a UTF-8 string from a file and print it
  $txt = file_get_contents('texto_contrato_port.txt');
  $txt_edit_name = str_replace("_nomedovisitante", $name, $txt);
  $txt_edit_rg = str_replace("_rg", $rg, $txt_edit_name);
  $txt_edit_day = str_replace("_dia", date("d"), $txt_edit_rg);
  $txt_edit_month = str_replace("_mes", $month, $txt_edit_day);
  $txt_edit_year = str_replace("_ano", date("Y"), $txt_edit_month);

  $pdf->Write(8, $txt_edit_year);

  $xmlFile = 'registros.xml';

  $special_chars = array("á", "é", "í", "ó", "ú", "Á", "É", "Í", "Ó", "Ú", "ñ", "Ñ", "ç", "Ç", "ã", "Ã", "ê", "Ê", "ô", "Ô", "õ", "Ô", "à", "À", "â", "Â");
  $replacement_chars  = array("a", "e", "i", "o", "u", "A", "E", "I", "O", "U", "n", "N", "c", "C", "a", "A", "e", "E", "o", "O", "o", "O", "a", "A", "a", "A");
  $replaced_name = str_replace($special_chars, $replacement_chars, $name);

  $nameForVideoFile = str_replace(' ', '', $replaced_name);

  $videoFileName = $nameForVideoFile . date("Y-m-d-h-i");

  $pdf->Output("pdfs/" . $videoFileName . ".pdf", "F");

  $xml = simplexml_load_file($xmlFile);

  $newPersonData = $xml->addChild('pessoa');
  $newPersonData->addChild('nome', $replaced_name);
  $newPersonData->addChild('phone', $phone);
  $newPersonData->addChild('facebook', $facebook);
  $newPersonData->addChild('videoFileName', $videoFileName);

  //Format XML to save indented tree rather than one line
  $dom = new DOMDocument('1.0');
  $dom->preserveWhiteSpace = false;
  $dom->formatOutput = true;
  $dom->loadXML($xml->asXML());

  if($dom->save('registros.xml')){
      header("Location: 4_port.html");
  }
  else{
      header("Location: erro.html");
  }
}

?>
