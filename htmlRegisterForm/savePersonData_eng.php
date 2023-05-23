<?php
require('tfpdf.php');

$name = $_POST['name'];
$rg = $_POST['rg'];
$phone = $_POST['phone'];

if($name == ""|| $phone == "" || $rg == ""){
    header("Location: requestInfo.html");
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
        $month = "January";
        break;
    case 2:
        $month = "February";
        break;
    case 3:
        $month = "March";
        break;
    case 4:
        $month = "April";
        break;
    case 5:
        $month = "May";
        break;
    case 6:
        $month = "June";
        break;
    case 7:
        $month = "July";
        break;
    case 8:
        $month = "August";
        break;
    case 9:
        $month = "September";
        break;
    case 10:
        $month = "October";
        break;
    case 11:
        $month = "November";
        break;
    case 12:
        $month = "December";
        break;
    default:
        $month = "January";
  }

  // Load a UTF-8 string from a file and print it
  $txt = file_get_contents('texto_contrato_ing.txt');
  $txt_edit_name = str_replace("_visitorname", $name, $txt);
  $txt_edit_rg = str_replace("_rg", $rg, $txt_edit_name);
  $txt_edit_day = str_replace("_day", date("d"), $txt_edit_rg);
  $txt_edit_month = str_replace("_month", $month, $txt_edit_day);
  $txt_edit_year = str_replace("_year", date("Y"), $txt_edit_month);

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
  $newPersonData->addChild('rg', $rg);
  $newPersonData->addChild('videoFileName', $videoFileName);

  //Format XML to save indented tree rather than one line
  $dom = new DOMDocument('1.0');
  $dom->preserveWhiteSpace = false;
  $dom->formatOutput = true;
  $dom->loadXML($xml->asXML());


  if($dom->save('registros.xml')){
      header("Location: 4_eng.html");
  }
  else{
      header("Location: error.html");
  }
}

?>
