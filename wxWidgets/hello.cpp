#include "wx/wx.h"

// g++ hello.cpp -o hello `wx-config --cxxflags --libs`

class Application : public wxApp {
public:
    virtual bool OnInit();
};

IMPLEMENT_APP( Application )

class Frame : public wxFrame {
public:
    Frame( const wxString &title, const wxPoint &pos, const wxSize &size );
    void OnButtonPress( wxCommandEvent &event );
private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE( Frame, wxFrame )
    EVT_BUTTON( wxID_CLOSE, Frame::OnButtonPress )
END_EVENT_TABLE()

bool Application::OnInit() 
{
    Frame *frame = new Frame( "HelloCode", wxPoint( 100, 100 ),
        wxSize( 200, 100 ) );
    frame->Show( true );
    return true;
}

Frame::Frame( const wxString &title, const wxPoint &pos, const wxSize &size ) : 
    wxFrame( NULL, wxID_ANY, title, pos, size ) 
{
    wxSizer *sizer = new wxBoxSizer( wxVERTICAL );
    this->SetSizer( sizer );
    wxButton *button = new wxButton( this, wxID_CLOSE, "Click Me" );
    sizer->Add( button, 0, wxALL, 10 );
}

void Frame::OnButtonPress( wxCommandEvent &event ) 
{
    wxMessageBox( "Hello World!", "HelloBox", wxOK | wxICON_INFORMATION );
    Close( true );
}