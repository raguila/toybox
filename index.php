<html>
	<head>
		<title>Sample Project Interface</title>
	
		<link rel="stylesheet" href="devtools/style.css">
		<script type="text/javascript" src="devtools/jquery-2.1.1.min.js"></script>
		<script type="text/javascript" src="devtools/jquery-linedtextarea.js"></script>
		<script>
		$(function() {
			$(".lined").linedtextarea(
				{}
			);
		});
		function addContent(query_input, content) {
     		document.getElementById(query_input).innerHTML = content;
		}
		</script>
	
	</head>
	<body>
		<header>
			BANNER HERE
		</header>
		<section class="container-fluid">
		
			<?php
				// define variable and set to empty value
				$temp=NULL;
					ini_set('display_errors', 1); // set to 0 for production version 
			error_reporting(E_ALL);
				if ($_SERVER["REQUEST_METHOD"] == "POST") {
				   $temp = test_input($_POST["query_input"]);
				  	write_to_file($temp);
				 echo("end of function CREATED!");

				  
				}

				function test_input($data) {
				  $data = trim($data);
				  $data = stripslashes($data);
				  $data = htmlspecialchars($data);
					
				
				  return $data;
				}

				function write_to_file($data){
					//$file = 'sample2.txt';
					//$data = 'this is your string to write';
					//file_put_contents("sample2.txt", "sampledata");
	
					
					
					$myfile = fopen("/var/www/html/214Project/sample2.txt", "w") or die("Unable to open file!");
					
					if (!is_writable("/var/www/html/214Project/sample2.txt")) {
   
                		chmod($myfile, 0755);  //this gives true 
                		echo("moded");
   						
					} else {
   						 echo 'The file is writable';
					}
					$data = htmlspecialchars_decode($data);
					fwrite($myfile, $data);
					fclose($myfile);
					echo("FILE CREATED!");

				}
				
				?>

			<div class="codeArea">
				
				<form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>" name="myForm">
					<textarea name="query_input" id="query_input" class="lined"><?php echo($temp);?></textarea>
					<!--<textarea name="query_input" id="query_input" class="lined"><?php echo $temp; ?></textarea>-->
					<input type="submit" id="runCode" value="RUN" />
					<input type="reset" id="resetCode" value="CLEAR"/>
					<input type="button" id="clearCode" value="NEW" onClick="addContent('query_input', '');"/>
					
				</form>
			</div>
			
			<div class="result">
				<?php
					if(empty($temp)){
						echo("hi");

					}else{
					exec('./samp "'.$temp.'" ', $out);
					//exec('hello.exe "'.$var.'" "'.$var2.'"',$out);
					//exec('./hello "'.$var.'" "'.$var2.'"',$out);
					print_r($out);
					}
				?>
			</div>
			
		</section>
	</body>

</html>