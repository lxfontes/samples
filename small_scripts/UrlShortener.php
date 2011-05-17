<?
class UrlShortener{
	var $base = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	function __constructor(){
	}

    /* convert bases int -> ourbase */
	function shortId($integer)
	{
		$length = strlen($this->base);
		while($integer > $length - 1)
		{
			$out = $this->base[fmod($integer, $length)] . $out;
			$integer = floor( $integer / $length );
		}
		return $this->base[$integer] . $out;
	}

	/* go from ourbase -> int */
	function expandId($string)
	{
		$length = strlen($this->base);
		$size = strlen($string) - 1;
		$string = str_split($string);
		$out = strpos($this->base, array_pop($string));
		foreach($string as $i => $char)
		{
			$out += strpos($this->base, $char) * pow($length, $size - $i);
		}
		return $out;
	}
}

/* example
 * $x = new UrlShortener();
 * echo $x->shortId(1234)."\n";
 * echo $x->expandId('jU')."\n";
 */
?>
