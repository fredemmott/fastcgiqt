<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
	version="2.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
	<xsl:output method="xhtml" indent="yes" />
	<xsl:template match="/dump">
		<html>
			<h1>Data</h1>
			<h2>GET data:</h2>
			<dl>
				<xsl:for-each select='//get/variable'>
					<dn><xsl:value-of select='name' /></dn>
					<dd><xsl:value-of select='value' /></dd>
				</xsl:for-each>
			</dl>
			<h2>POST data:</h2>
			<dl>
				<xsl:for-each select='//post/variable'>
					<dn><xsl:value-of select='name' /></dn>
					<dd><xsl:value-of select='value' /></dd>
				</xsl:for-each>
			</dl>
			<h2>Headers:</h2>
			<dl>
				<xsl:for-each select='//server/variable'>
					<dn><xsl:value-of select='name' /></dn>
					<dd><xsl:value-of select='value' /></dd>
				</xsl:for-each>
			</dl>
		</html>
	</xsl:template>
</xsl:stylesheet>
