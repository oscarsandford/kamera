#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <kkamera.h>


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
		// Hold onto the imported image for edits and exports.
		// Should try to make this not a class-scope variable later.
		cv::Mat current_image;
		// I can't figure out how to get the value of the slider from the wxCommandEvent
		// passed into its handler, so a class-wide array of sliders is a temporary fix.
		wxSlider *sliders[SLIDER_COUNT];
	private:
		void OnImport(wxCommandEvent &);
		void OnExport(wxCommandEvent &);
		void OnExit(wxCommandEvent &);
		void OnContrastSliderMove(wxCommandEvent &);
		void OnBrightnessSliderMove(wxCommandEvent &);
		wxDECLARE_EVENT_TABLE();
};

// Program entry point.
// Set up events and start up app.
wxBEGIN_EVENT_TABLE(KFrame, wxFrame)
	EVT_MENU(1, KFrame::OnImport)
	EVT_MENU(2, KFrame::OnExport)
	EVT_MENU(wxID_EXIT, KFrame::OnExit)
	EVT_SLIDER(wxID_ANY, KFrame::OnContrastSliderMove)
	EVT_SLIDER(wxID_ANY, KFrame::OnBrightnessSliderMove)
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

	// Bottom panel (where the contrast, brightness, and other controls are)
	wxPanel *bottom_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(600,200));
	bottom_panel->SetBackgroundColour(wxColor(200, 150, 150));

	// Control slider for all the adjustments
	wxBoxSizer *control_sizer = new wxBoxSizer(wxHORIZONTAL);
	// Containers for sliders and respective text labels
	wxBoxSizer *contrast_sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *brightness_sizer = new wxBoxSizer(wxVERTICAL);

	// Sliders and text.
	KFrame::sliders[CONTRAST] = new wxSlider(bottom_panel, wxID_ANY, 10, 1, 100, wxDefaultPosition, wxSize(100,100), wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
	wxStaticText *txt1 = new wxStaticText(bottom_panel, wxID_ANY, "Contrast", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	KFrame::sliders[BRIGHTNESS] = new wxSlider(bottom_panel, wxID_ANY, 10, 1, 100, wxDefaultPosition, wxSize(100,100), wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
	wxStaticText *txt2 = new wxStaticText(bottom_panel, wxID_ANY, "Brightness", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);

	// Add sliders and labels to control sizer
	contrast_sizer->Add(txt1, 0, wxALIGN_CENTER_HORIZONTAL);
	contrast_sizer->Add(KFrame::sliders[CONTRAST], 1, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxRIGHT, 10);
	control_sizer->Add(contrast_sizer, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

	brightness_sizer->Add(txt2, 0, wxALIGN_CENTER_HORIZONTAL);
	brightness_sizer->Add(KFrame::sliders[BRIGHTNESS], 1, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxRIGHT, 10);
	control_sizer->Add(brightness_sizer, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
	
	// Bind slider event handlers
	KFrame::sliders[CONTRAST]->Bind(wxEVT_SLIDER, &KFrame::OnContrastSliderMove, this);
	KFrame::sliders[BRIGHTNESS]->Bind(wxEVT_SLIDER, &KFrame::OnBrightnessSliderMove, this);
	// Finally set panel's sizer to the control sizer
	bottom_panel->SetSizer(control_sizer);

	// Main sizer (container)
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	// Add the main panels to the main container.
	// (Top panel has twice proportion of bottom panel, margin 8.)
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
	// TODO: make a help popup with some tips on how to do things.

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
	KFrame::current_image = import_image(path);
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

	// Get the current values of the style sliders.
	float raw = (float)KFrame::sliders[CONTRAST]->GetValue();
	float cv = (raw/100)*3;
	float bv = (int)KFrame::sliders[BRIGHTNESS]->GetValue();
	KFrame::current_image = adjust_contrast_and_brightness(KFrame::current_image, cv, bv);
	// Call function to export the image by path.
	export_image(current_image, path);
}

// These event handlers are pretty unnecessary because I have the sliders present across 
// the scope of the class. They're still here because I may come up with another solution.

// Fired when the contrast slider is adjusted.
void KFrame::OnContrastSliderMove(wxCommandEvent &e) 
{
	int raw = (int)KFrame::sliders[CONTRAST]->GetValue();
	float contrast_value = ((float)raw/100)*3;
	std::cout << "contrast slider: " << contrast_value << std::endl;
}

// Fired when the contrast slider is adjusted.
void KFrame::OnBrightnessSliderMove(wxCommandEvent &e) 
{
	float brightness_value = (float)KFrame::sliders[BRIGHTNESS]->GetValue();
	std::cout << "brightness slider: " << brightness_value << std::endl;
}