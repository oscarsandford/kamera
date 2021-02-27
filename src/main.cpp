#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <kkamera.h>

cv::Mat current_image;

// Class definitions
class Kamera : public wxApp
{
	public:
		virtual bool OnInit();
};
class KFrame : public wxFrame
{
	public:
		KFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	// Frame events and their function prototypes.
	private:
		void OnImport(wxCommandEvent &WXUNUSED(e));
		void OnExport(wxCommandEvent &WXUNUSED(e));
		void OnExit(wxCommandEvent &WXUNUSED(e));
		wxDECLARE_EVENT_TABLE();
};

// Program entry point.
// Set up events and start up app.
wxBEGIN_EVENT_TABLE(KFrame, wxFrame)
	EVT_MENU(1, KFrame::OnImport)
	EVT_MENU(2, KFrame::OnExport)
	EVT_MENU(wxID_EXIT, KFrame::OnExit)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(Kamera);

// Initializes here.
bool Kamera::OnInit()
{
	KFrame *kf = new KFrame("Kamera Studio", wxDefaultPosition, wxDefaultSize);
	kf->Show(true);
	return true;
}

KFrame::KFrame(const wxString &title, const wxPoint &pos, const wxSize &size) 
	: wxFrame(nullptr, wxID_ANY, title, pos, size)
{
	// Top panel (where the image is previewed)
	wxPanel *top_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(600,200));
	top_panel->SetBackgroundColour(wxColor(150, 200, 150));

	// Bottom panel (where the contrast, brightness, etc controls are)
	wxPanel *bottom_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(600,200));
	bottom_panel->SetBackgroundColour(wxColor(200, 150, 150));

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	// Top panel has twice proportion of bottom panel, margin 10
	sizer->Add(top_panel, 2, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 8);
	sizer->Add(bottom_panel, 1, wxEXPAND | wxALL, 8);
	this->SetSizerAndFit(sizer);

	// Menus and menu bar
	wxMenu *file_menu = new wxMenu;
	file_menu->Append(1, "Import JPG");
	file_menu->Append(2, "Export as JPG");
	file_menu->AppendSeparator();
	file_menu->Append(wxID_EXIT);

	wxMenu *help_menu = new wxMenu;
	
	wxMenuBar *menu_bar = new wxMenuBar;
	menu_bar->Append(file_menu, "File");
	menu_bar->Append(help_menu, "Help");

	SetMenuBar(menu_bar);
	CreateStatusBar();
	SetStatusText("Kamera (Dev)");
}

// Kill the app when "Quit" selected.
void KFrame::OnExit(wxCommandEvent &WXUNUSED(e))
{
	Close(true);
}

// Fired when "Import" selected in "File" menu dropdown.
void KFrame::OnImport(wxCommandEvent &WXUNUSED(e))
{
	// Open dialogue for user to select a file to import.
	wxFileDialog importFileDialog(this, 
		_("Open JPG file"), "", "", "JPG files (*.jpg)|*.jpg", 
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);
	if (importFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}
	// Check the file path defined by the user in the dialog
	wxFileInputStream input_stream(importFileDialog.GetPath());
	if (!input_stream.IsOk()) {
		wxLogError("Cannot open %s", importFileDialog.GetPath());
		return;
	}
	// Derive the C string of the path.
	char path[1024];
	strncpy(path, (const char *)importFileDialog.GetPath().mb_str(wxConvUTF8), 1023);

	// Call function to create the image Mat by path.
	current_image = import_image(path);
}

// Fired when "Export" selected in "File" menu dropdown.
void KFrame::OnExport(wxCommandEvent &WXUNUSED(e))
{
	// Open dialogue for user to export current file.
	wxFileDialog exportFileDialog(this, 
		_("Save image as JPG"), "", "", "JPG files (*.jpg)|*.jpg", 
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
	);
	if (exportFileDialog.ShowModal() == wxID_CANCEL) {
		return;
	}
	// Check the file path defined by the user in the dialog.
	wxFileOutputStream output_stream(exportFileDialog.GetPath());
	if (!output_stream.IsOk()) {
		wxLogError("Cannot save to %s", exportFileDialog.GetPath());
		return;
	}
	// Derive the C string of the path.
	char path[1024];
	strncpy(path, (const char *)exportFileDialog.GetPath().mb_str(wxConvUTF8), 1023);

	// Call function to export the image by path.
	export_image(current_image, path);
}