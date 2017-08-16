<?php
$url = $file_name;
$fields = array(
            '__VIEWSTATE'=>urlencode($state),
            '__EVENTVALIDATION'=>urlencode($valid),
            'btnSubmit'=>urlencode('Submit')
        );
 
//url-ify the data for the POST
foreach($fields as $key=>$value) { $fields_string .= $key.'='.$value.'&'; }
$fields_string = rtrim($fields_string,'&');
 
//open connection
$ch = curl_init();
 
//set the url, number of POST vars, POST data
curl_setopt($ch,CURLOPT_URL,$url);
curl_setopt($ch,CURLOPT_POST,count($fields));
curl_setopt($ch,CURLOPT_POSTFIELDS,$fields_string);
 
//execute post
$result = curl_exec($ch);
print $result;
?>