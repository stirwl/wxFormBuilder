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
//   Ryan Mulder - rjmyst3@gmail.com
//
///////////////////////////////////////////////////////////////////////////////

#include "dataobject.h"

#include <cstring>

#include <ticpp.h>

#include "model/objectbase.h"
#include "rad/appdata.h"
#include "utils/typeconv.h"


wxDataFormat wxFBDataObject::DataObjectFormat()
{
    static const auto format = wxDataFormat("wxFormBuilderDataFormat");

    return format;
}


wxFBDataObject::wxFBDataObject(PObjectBase object)
{
    if (object) {
        // create xml representation of ObjectBase
        ticpp::Element element;
        object->SerializeObject(&element);

        // add version info to xml data, just in case it is pasted into a different version of wxFB
        element.SetAttribute("fbp_version_major", AppData()->m_fbpVerMajor);
        element.SetAttribute("fbp_version_minor", AppData()->m_fbpVerMinor);

        ticpp::Document doc;
        doc.LinkEndChild(&element);
        TiXmlPrinter printer;
        printer.SetIndent("\t");

        printer.SetLineBreak("\n");

        doc.Accept(&printer);
        m_data = printer.Str();
    }
}


PObjectBase wxFBDataObject::GetObject() const
{
    if (m_data.empty()) {
        return PObjectBase();
    }

    // Read Object from xml
    try {
        ticpp::Document doc;
        doc.Parse(m_data, true, TIXML_ENCODING_UTF8);
        ticpp::Element* element = doc.FirstChildElement();


        int major, minor;
        element->GetAttribute("fbp_version_major", &major);
        element->GetAttribute("fbp_version_minor", &minor);

        if (
          major > AppData()->m_fbpVerMajor || (AppData()->m_fbpVerMajor == major && minor > AppData()->m_fbpVerMinor)) {
            wxLogError(_("This object cannot be pasted because it is from a newer version of wxFormBuilder"));
        }

        if (
          major < AppData()->m_fbpVerMajor || (AppData()->m_fbpVerMajor == major && minor < AppData()->m_fbpVerMinor)) {
            AppData()->ConvertObject(element, major, minor);
        }

        PObjectDatabase db = AppData()->GetObjectDatabase();
        return db->CreateObject(element);
    } catch (ticpp::Exception& ex) {
        wxLogError(_WXSTR(ex.m_details));
        return PObjectBase();
    }
}


void wxFBDataObject::GetAllFormats(wxDataFormat* formats, Direction dir) const
{
    switch (dir) {
        case Get:
            formats[0] = DataObjectFormat();
            formats[1] = wxDF_TEXT;
            break;
        case Set:
            formats[0] = DataObjectFormat();
            break;
        default:
            break;
    }
}

bool wxFBDataObject::GetDataHere(const wxDataFormat&, void* buf) const
{
    if (!buf) {
        return false;
    }

    std::memcpy(buf, m_data.data(), m_data.size());

    return true;
}

size_t wxFBDataObject::GetDataSize([[maybe_unused]] const wxDataFormat& format) const
{
    return m_data.size();
}

size_t wxFBDataObject::GetFormatCount(Direction dir) const
{
    switch (dir) {
        case Get:
            return 2;
        case Set:
            return 1;
        default:
            return 0;
    }
}

wxDataFormat wxFBDataObject::GetPreferredFormat([[maybe_unused]] Direction dir) const
{
    return DataObjectFormat();
}

bool wxFBDataObject::SetData(const wxDataFormat& format, size_t len, const void* buf)
{
    if (format != DataObjectFormat()) {
        return false;
    }

    m_data.assign(reinterpret_cast<const char*>(buf), len);

    return true;
}
