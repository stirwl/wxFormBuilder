///////////////////////////////////////////////////////////////////////////////
//
// wxFormBuilder - A Visual Dialog Editor for wxWidgets.
// Copyright (C) 2005 José Antonio Hurtado
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// Written by
//   José Antonio Hurtado - joseantonio.hurtado@gmail.com
//   Juan Antonio Ortega  - jortegalalmolda@gmail.com
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CODEGEN_XRCCG_H
#define CODEGEN_XRCCG_H

#include <tinyxml2.h>

#include "codegen/codegen.h"


namespace ticpp
{
class Element;
}

/// XRC code generator.

class XrcCodeGenerator : public CodeGenerator
{
public:
    /// Configures the code writer for the XML file.
    void SetWriter(PCodeWriter cw);

    /// Generates the XRC code for the project.
    bool GenerateCode(PObjectBase project) override;

private:
    ticpp::Element* GetElement(PObjectBase obj, ticpp::Element* parent = NULL);
    tinyxml2::XMLElement* GetElement(PObjectBase object, tinyxml2::XMLElement* parent);

private:
    PCodeWriter m_cw;
    tinyxml2::XMLDocument m_xrc{false, tinyxml2::PRESERVE_WHITESPACE};
    std::vector<ticpp::Element*> m_contextMenus;
    // FIXME: Rename after removal of ticpp
    std::vector<tinyxml2::XMLElement*> m_contextMenus2;
};

#endif  // CODEGEN_XRCCG_H
