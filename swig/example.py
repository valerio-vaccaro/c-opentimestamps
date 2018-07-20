import ots

buf = bytearray(b'foo\x00')
ots.toHex(buf)

res = ucharp()
res = bytearray(b'ffwefwfefeffeeffegtegtfeffrfrerefrefreferrewrwrewrewoo\x00')
sha256 = ots.OpSha256()
sha256.call(buf,res)

