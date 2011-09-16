<?php

function dump_raw_bits($im, &$bytes)
{
	$w = imagesx($im);
	$h = imagesy($im);

	$n = 0;
	$dump = '';
	for ($j = 0; $j < $h; $j++)
	{
		for ($i = 0; $i < $w; $i++)	
		{
			$ind = imagecolorat($im,  $i,  $j);
			$dump .= ($ind ? '\0' : '\1' );
			$n++;
			if ($n > 40) 
			{
				$bytes += $n;
				$n = 0;
				$dump .= "\"\n  \"";
			} 	
		}
	}
	$bytes += $n;
	return $dump;
}

function dump_compressed_bits(&$im, &$bytes)
{
	$w = imagesx($im);
	$h = imagesy($im);

	$n = 0; $m = 0; $t = 0;
	$dump = '';
	for ($j = 0; $j < $h; $j++)
	{
		for ($i = 0; $i < $w; $i++)	
		{
			$ind = imagecolorat ( $im ,  $i ,  $j );
			if (!$ind) $t |= (0x01 << $m);
			//echo "$m. ".sprintf("%02x", $t)." = ".round($t)."\n";
			$m++;
			if ($m > 7)
			{
				$out = '\\x'. sprintf("%02x",$t);
				if ($t == 0) $out = '\\0';
				if ($t >= 32 && $t <= 33) $out = ''.chr($t);
				$t = 0;
				$m = 0;
				$dump .= $out;
				$n += strlen($out);			
			}
			if ($n > 70) 
			{
				$bytes += $n;
				$n = 0;
				$dump .= "\"\n  \"";
			} 
		}
	}
	$bytes += $n;
	return $dump;
}

if (!isset($argv[1])) {
	echo "Must provide bmp file as arg1\n";
	exit(-1);
}

$im = imagecreatefrombmp($argv[1]);
if (!$im) {
	echo "Unable to open font '".$argv[1]."'\n";
	exit(-1);
}


$c_name = 'font_source';
$char_w = 8;
$char_h = 8;

//header('Content-type: image/gif');
//imagegif($im);
imagexbm($im, "inline_font.xbm");
//exit;

$bytes = 0;
$dump = dump_compressed_bits($im, $bytes);

	$w = imagesx($im);
	$h = imagesy($im);

if (preg_match("/(\s+?)/", $argv[1], $mc)) {
	$c_name = $mc[1];
}

if (preg_match("/(\d)x(\d)/", $argv[1], $mc)) {
	$char_w = $mc[1];
	$char_h = $mc[2];
} else if (preg_match("/(\d)/", $argv[1], $mc)) {
	$char_w = $w / 16;
	$char_h = $mc[1];
}

/*echo '/* size of 1 letter: */
#define '.$c_name.'_w '.$char_w.'
#define '.$c_name.'_h '.$char_h;*/

echo '/* gimp-like .C struct: */
static const struct {
  Uint32  	 width;
  Uint32  	 height;
  Uint32  	 pixels_per_byte; 
  Uint8 	 pixel_data['.$bytes.'];
} '.$c_name.' = {
  '.$w.', '.$h.', 8,
  "';

echo $dump;

echo "\"\n";

echo "\n";
echo "  };\n";	//'"


/* I took this function from the internet and modified it slightly,
 * but now I can't find the original source :(
 *  if you know where it came from, please send me a word
 * will probably not work with non-1bpp files.. 
 */
function imagecreatefrombmp($filename) 
{
	$header = @file_get_contents ($filename, false, NULL, -1, 54);
	if (strlen($header) < 54 || /* Bitmap header is at least 54 bytes */ 
		substr($header, 0, 2) != 'BM') /* First 2 bytes are 0x424d */ 
		return NULL; /* Invalid bitmap */
	foreach (array( /* Read fields by sizes and offsets */ 
		array('width', 4, 18),
		array('height', 4, 22),
		array('bpp',  2, 28),
		array('offset', 4, 10),
	) as $array) {
		$temp = unpack("v", /* unsigned 16 bit, little endian */
			substr($header, $array[2], $array[1]));
		${$array[0]} = $temp[1];
	}
	$body = file_get_contents($filename, false, NULL, $offset);
	if ($bpp <= 8) 	$image = imagecreate($width, $height);
	else			$image = imagecreatetruecolor($width, $height);
	$x = 0; $y = 1; $len = strlen($body);
	$pad = 0;
	switch ($bpp) {
		case 1:
		$colors = array(
			imagecolorallocate($image, 255, 255, 255),
			imagecolorallocate($image, 0, 0, 0),
		);	
		for ($i = 0; $i < strlen($body); $i++) {
			$bin = sprintf("%08b", ord($body[$i]));
			for ($j = 0; $j < 8; $j++)
				imagesetpixel($image, $x + $j, $height - $y, $bin[$j]);
			$x += $j;
			if ($x >= $width) 
			{
				$i += $pad;
				$x = 0;
				$y++;
			}
		}
		break;	
		default:
		for ($i = 0; $i < $len; $i++) {
			$b = ord($body[$i]);
			$g = ord($body[$i+1]);
			$r = ord($body[$i+2]);

			$color = imagecolorallocate($image, $r, $g, $b); 
			imagesetpixel($image, $x, $height - $y, $color); 
			//imagesetpixel($image, $x + $j, $height - $y, $bin[$j]);			
			if ($x >= $width) 
			{
				$i += $pad;
				$x = 0;
				$y++;//OR USE? if (++$y > $height) break;
			}
		}

		//    Calculate and draw the pixel 
		$color    =    imagecolorallocate($image,$r,$g,$b); 
		imagesetpixel($image,$x,$height-$y,$color); 

		//    Raise the horizontal position 
		$x++; 		
		break;
	}	
	return $image;	
}



?>