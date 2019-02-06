/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.ti.msp.descriptortool;

import javax.xml.parsers.DocumentBuilder; 
import javax.xml.parsers.DocumentBuilderFactory;
import org.xml.sax.ErrorHandler;
import org.xml.sax.SAXException; 
import org.xml.sax.SAXParseException;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 *
 * @author a0272979
 */
public class XMLDOM {
    static final String outputEncoding = "UTF-8";
    Document doc;

    XMLDOM(String filePath) throws Exception  {
        String filename = filePath;
        boolean ignoreWhitespace = false;
        boolean ignoreComments = false;
        boolean putCDATAIntoText = true;
        boolean createEntityRefs = false;
        
        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        DocumentBuilder db = dbf.newDocumentBuilder();
        dbf.setNamespaceAware(true);
        dbf.setIgnoringComments(ignoreComments);
        dbf.setIgnoringElementContentWhitespace(ignoreWhitespace);
        dbf.setCoalescing(putCDATAIntoText);
        dbf.setExpandEntityReferences(!createEntityRefs);
        OutputStreamWriter errorWriter = new OutputStreamWriter(System.err,
                                         outputEncoding);
        db.setErrorHandler(new MyErrorHandler (new PrintWriter(errorWriter, true)));
        this.doc = db.parse(new File(filename));   
        
    }

    private static class MyErrorHandler implements ErrorHandler {
     
        private PrintWriter out;

        MyErrorHandler(PrintWriter out) {
            this.out = out;
        }

        private String getParseExceptionInfo(SAXParseException spe) {
            String systemId = spe.getSystemId();
            if (systemId == null) {
                systemId = "null";
            }

            String info = "URI=" + systemId + " Line=" + spe.getLineNumber() +
                          ": " + spe.getMessage();
            return info;
        }

        public void warning(SAXParseException spe) throws SAXException {
            out.println("Warning: " + getParseExceptionInfo(spe));
        }

        public void error(SAXParseException spe) throws SAXException {
            String message = "Error: " + getParseExceptionInfo(spe);
            throw new SAXException(message);
        }

        public void fatalError(SAXParseException spe) throws SAXException {
            String message = "Fatal Error: " + getParseExceptionInfo(spe);
            throw new SAXException(message);
        }
    }
    
        /**
     * Find the named subnode in a node's sublist.
     * <ul>
     * <li>Ignores comments and processing instructions.
     * <li>Ignores TEXT nodes (likely to exist and contain
     *         ignorable whitespace, if not validating.
     * <li>Ignores CDATA nodes and EntityRef nodes.
     * <li>Examines element nodes to find one with
     *        the specified name.
     * </ul>
     * @param name  the tag name for the element to find
     * @param node  the element node to start searching from
     * @return the Node found
     */
    public Node findNode(String name) {
        Node node = doc.getDocumentElement();

        NodeList list = node.getChildNodes();
        for (int i=0; i < list.getLength(); i++) {
            Node subnode = list.item(i);
            if (subnode.getNodeType() == Node.ELEMENT_NODE) {
               if (subnode.getNodeName().equals(name)) 
                   return subnode;
            }
        }
        return null;
    }

    /**
     * Find the named subnode in a node's sublist.
     * <ul>
     * <li>Ignores comments and processing instructions.
     * <li>Ignores TEXT nodes (likely to exist and contain
     *         ignorable whitespace, if not validating.
     * <li>Ignores CDATA nodes and EntityRef nodes.
     * <li>Examines element nodes to find one with
     *        the specified name.
     * </ul>
     * @param name  the tag name for the element to find
     * @param node  the element node to start searching from
     * @return the Node found
     */
    public Node findSubNode(String name, Node node) {
        if (node.getNodeType() != Node.ELEMENT_NODE) {
            System.err.println("Error: Search node not of element type");
            System.exit(22);
        }

        if (! node.hasChildNodes()){
            return null;
        }

        NodeList list = node.getChildNodes();
        for (int i=0; i < list.getLength(); i++) {
            Node subnode = list.item(i);
            if (subnode.getNodeType() == Node.ELEMENT_NODE) {
               if (subnode.getNodeName().equals(name)) 
                   return subnode;
            }
        }
        return null;
    }
    
 /**
  * Return the text that a node contains. This routine:
  * <ul>
  * <li>Ignores comments and processing instructions.
  * <li>Concatenates TEXT nodes, CDATA nodes, and the results of
  *     recursively processing EntityRef nodes.
  * <li>Ignores any element nodes in the sublist.
  *     (Other possible options are to recurse into element 
  *      sublists or throw an exception.)
  * </ul>
  * @param    node  a  DOM node
  * @return   a String representing its contents
  */
    public String getText(Node node) {
        StringBuffer result = new StringBuffer();
        if (! node.hasChildNodes()) return "";

        NodeList list = node.getChildNodes();
        for (int i=0; i < list.getLength(); i++) {
            Node subnode = list.item(i);
            if (subnode.getNodeType() == Node.TEXT_NODE) {
                result.append(subnode.getNodeValue());
            }
            else if (subnode.getNodeType() == Node.CDATA_SECTION_NODE) {
                result.append(subnode.getNodeValue());
            }
            else if (subnode.getNodeType() == Node.ENTITY_REFERENCE_NODE) {
                // Recurse into the subtree for text
                // (and ignore comments)
                result.append(getText(subnode));
            }
        }

        return result.toString();
    }
    
 /**
  * Returns attribute named on specified node. 
  * @param    string  attribute name
  * @param    node  a  DOM node
  * @return   a String representing its contents
  */
    public String getAttribute(String string, Node node) {
        NamedNodeMap attr = node.getAttributes();
        for (int i=0; i < attr.getLength(); i++) {
            Node subnode = attr.item(i);
            if (subnode.getNodeName().contentEquals(string)) {
                return (subnode.getNodeValue());
            }
        }

        return null;
    }
    
    

    
}
//Released_Version_5_20_06_02
