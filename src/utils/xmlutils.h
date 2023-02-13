#ifndef UTILS_XMLUTILS_H
#define UTILS_XMLUTILS_H

#include <memory>

#include <tinyxml2.h>
#include <wx/string.h>


namespace XMLUtils
{

/**
 * @brief Load the specified XML file
 *
 * The file must use UTF-8 encoding, this is not verified and simply assumed.
 *
 * @param path Path to XML file
 * @param collapseWhitespace If true, collapse whitespace, otherwise preserve it
 * @return The loaded XML file
 *
 * @throw wxFBException Failed to open or parse the file
 */
std::unique_ptr<tinyxml2::XMLDocument> LoadXMLFile(const wxString& path, bool collapseWhitespace = false);

/**
 * @brief Return string typed attribute
 *
 * @param element XML element node
 * @param name Name of the attribute
 * @param defaultValue Default value returned if attribute is missing
 * @return Attribute value
 */
wxString StringAttribute(const tinyxml2::XMLElement* element, const wxString& name, const wxString& defaultValue = wxEmptyString);

/**
 * @brief Return text of the element
 *
 * @param element XML element node
 * @param defaultValue Default value returned if element contains no text
 * @param deepSearch If true, searches for the first text node, otherwise returns the text of the first child node.
 *                   If the first child node is not a text node the default value will be returned.
 * @return Text of the element
 */
wxString GetText(const tinyxml2::XMLElement* element, const wxString& defaultValue = wxEmptyString, bool deepSearch = false);

}  // namespace XMLUtils

#endif  // UTILS_XMLUTILS_H
