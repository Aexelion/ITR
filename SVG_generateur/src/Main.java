import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;

import javax.xml.parsers.SAXParser;

import javax.xml.parsers.SAXParserFactory;

import javax.xml.transform.TransformerFactoryConfigurationError;

import org.w3c.dom.DOMException;

import org.xml.sax.SAXException;

public class Main {
	
	public int getWCET(String name, SAXParser p){
		
		return -1;
	}
	
	public static void main(String arg[]){
		try {

	         SAXParserFactory factory = SAXParserFactory.newInstance();

	         SAXParser parser = factory.newSAXParser();

	         

	         parser.parse("TP2_exe-res.xml", new MyXMLHandler());


	      } catch (DOMException e) {

	         e.printStackTrace();

	      } catch (ParserConfigurationException e) {

	         e.printStackTrace();

	      } catch (TransformerFactoryConfigurationError e) {

	         e.printStackTrace();

	      } catch (SAXException e) {

	         e.printStackTrace();

	      } catch (IOException e) {

	         // TODO Auto-generated catch block

	         e.printStackTrace();

	      }
		
	}
}
