#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <kkamera.h>
// Need this for strlen and strstr.
#include <cstring>


// TODO: split classes and functionality apart
// into separate source code files, then include them
// in this main.cpp, which serves to initialize only.

// Class definitions
class Kamera : public wxApp
{
	public:
		virtual bool OnInit();
};
class KFrame : public wxFrame
{
	public:
		KFrame(const wxString &title, const wxPoint &, const wxSize &);
		wxSlider *sliders[SLIDER_COUNT];
	private:
		void OnImport(wxCommandEvent &);
		void OnExport(wxCommandEvent &);
		void OnExit(wxCommandEvent &);
		void OnContrastSliderMove(wxCommandEvent &);
		void OnBrightnessSliderMove(wxCommandEvent &);
		wxDECLARE_EVENT_TABLE();
};
class KPreview : public wxPanel 
{
	public:
		KPreview(KFrame *, wxString file, wxBitmapType format);
		wxImage img_wxobj;
		wxBitmap img_bitmap;
		cv::Mat img_mat;
		int width, height;
		void LoadNewPreviewImage(wxString file, wxBitmapType format);
		void ForceRender();
		wxDECLARE_EVENT_TABLE();
	private:
		void OnPaintEvent(wxPaintEvent &);
		void OnPreviewResize(wxSizeEvent &);
		// DC means device context, for rendering the preview image.
		void RenderPreview(wxDC &);
};

// A silly fix, but I need to get the import method in KFrame to call a method 
// that belonds to KPreview, and above all initialize it in the KFrame constructor.
// I'll try to come back to this, or just redesign the two classes entirely.
KPreview *editor_preview;


// Program entry point.
// Set up event tables for classes and start up app.
wxBEGIN_EVENT_TABLE(KFrame, wxFrame)
	EVT_MENU(1, KFrame::OnImport)
	EVT_MENU(2, KFrame::OnExport)
	EVT_MENU(wxID_EXIT, KFrame::OnExit)
	EVT_SLIDER(wxID_ANY, KFrame::OnContrastSliderMove)
	EVT_SLIDER(wxID_ANY, KFrame::OnBrightnessSliderMove)
wxEND_EVENT_TABLE()
wxBEGIN_EVENT_TABLE(KPreview, wxPanel)
	EVT_PAINT(KPreview::OnPaintEvent)
	EVT_SIZE(KPreview::OnPreviewResize)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(Kamera);


// Initializes here.
bool Kamera::OnInit()
{
	wxInitAllImageHandlers();
	KFrame *kf = new KFrame("Kamera Studio", wxDefaultPosition, wxDefaultSize);
	kf->Show(true);
	return true;
}


KPreview::KPreview(KFrame *parent, wxString file, wxBitmapType format) 
	: wxPanel(parent)
{
	this->LoadNewPreviewImage(file, format);
}

// Update the preview image. Typically this will be called after an import.
void KPreview::LoadNewPreviewImage(wxString file, wxBitmapType format) 
{
	// Prevents us looking for a file on startup.
	if (strlen((const char *)file.mb_str(wxConvUTF8)) > 0) 
	{
		// TODO: check if file loaded correctly
		img_wxobj.LoadFile(file, format);
		KPreview::width = img_wxobj.GetWidth();
		KPreview::height = img_wxobj.GetHeight();
	}
}

// Call a non-signal-based render. Useful for initial import, which
// does not trigger a wxPaintEvent.
void KPreview::ForceRender() 
{
	wxClientDC dc(this);
	KPreview::RenderPreview(dc);
}

// Render device context whenever the KPreview instance is resized.
void KPreview::OnPaintEvent(wxPaintEvent &WXUNUSED(e)) 
{
	wxClientDC dc(this);
	KPreview::RenderPreview(dc);
}

// Refresh the panel to paint itself again, so the image is resized when the panel is.
void KPreview::OnPreviewResize(wxSizeEvent &e) 
{
	Refresh();
	e.Skip();
}

// Draw the image based on current state of preview_image_bitmap
void KPreview::RenderPreview(wxDC &dc) 
{
	if (img_wxobj.IsOk()) 
	{
		int new_width, new_height;
		dc.GetSize(&new_width, &new_height);
		// Scale and update bitmap if anything changed
		// TODO: scale based on aspect ratio
		if (new_width != width || new_height != height) 
		{
			img_bitmap = wxBitmap(img_wxobj.Scale(new_width, new_height));
			width = new_width;
			height = new_height;
		}
		dc.DrawBitmap(img_bitmap, 0, 0, false);
	}
}


// We construct a KFrame with all the goods inside. Might move some stuff up to OnInit, or 
// break it down further, but for right now it's a good, centralized place to make the GUI.
KFrame::KFrame(const wxString &title, const wxPoint &pos, const wxSize &size) 
	: wxFrame(nullptr, wxID_ANY, title, pos, size)
{
	// -> Top panel (where the image is previewed)
	wxPanel *top_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(600,200));
	top_panel->SetBackgroundColour(wxColor(150, 200, 150));

	// Image preview
	wxBoxSizer *preview_sizer = new wxBoxSizer(wxVERTICAL);
	// Target our happy little global variable. Don't display anything initially.
	editor_preview = new KPreview(this, _(""), wxBITMAP_TYPE_JPEG);
	// Change background color to same as panel so there isn't a visible difference.
	editor_preview->SetBackgroundColour(wxColor(150, 200, 150));
	preview_sizer->Add(editor_preview, 1, wxALIGN_CENTER_HORIZONTAL | wxEXPAND, 5);
	top_panel->SetSizer(preview_sizer);


	// -> Bottom panel (where the contrast, brightness, and other controls are)
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


	// -> Main sizer (container)
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
	if (importFileDialog.ShowModal() == wxID_CANCEL) 
	{
		return;
	}
	// Check the file path defined by the user in the dialog
	wxFileInputStream input_stream(importFileDialog.GetPath());
	if (!input_stream.IsOk()) 
	{
		wxLogError("Cannot open %s", importFileDialog.GetPath());
		return;
	}
	// Derive the C string of the path.
	char path[1024];
	strncpy(path, (const char *)importFileDialog.GetPath().mb_str(wxConvUTF8), 1023);

	// Call function to create the image Mat by path.
	editor_preview->img_mat = kk_import_image(path);

	// Bitmap type based on file extension
	// TODO: Add support for png files. This will depend how how well I can get 
	// the OpenCV library to manage them.
	if (strstr(path, ".png")) 
	{
		editor_preview->LoadNewPreviewImage(_(path), wxBITMAP_TYPE_PNG);
	}
	else 
	{
		editor_preview->LoadNewPreviewImage(_(path), wxBITMAP_TYPE_JPEG);
	}
	editor_preview->ForceRender();
}

// Fired when "Export" selected in "File" menu dropdown.
void KFrame::OnExport(wxCommandEvent &WXUNUSED(e))
{
	// Open dialogue for user to export current file.
	wxFileDialog exportFileDialog(this, 
		_("Save image as JPG"), "", "", "JPG files (*.jpg)|*.jpg", 
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
	);
	if (exportFileDialog.ShowModal() == wxID_CANCEL) 
	{
		return;
	}
	// Check the file path defined by the user in the dialog.
	wxFileOutputStream output_stream(exportFileDialog.GetPath());
	if (!output_stream.IsOk()) 
	{
		wxLogError("Cannot save to %s", exportFileDialog.GetPath());
		return;
	}
	// Derive the C string of the path.
	char path[1024];
	strncpy(path, (const char *)exportFileDialog.GetPath().mb_str(wxConvUTF8), 1023);

	// Get the current values of the style sliders.
	float raw = (int)KFrame::sliders[CONTRAST]->GetValue();
	float cv = ((float)raw/100)*3;
	float bv = (int)KFrame::sliders[BRIGHTNESS]->GetValue();
	editor_preview->img_mat = kk_adjust_contrast_and_brightness(editor_preview->img_mat, cv, bv);
	// Call function to export the image by path.
	kk_export_image(editor_preview->img_mat, path);
}

// These event handlers are here for when I want to make calls to the adjustment methods.
// They will update the main image Mat.

// Fired when the contrast slider is adjusted.
void KFrame::OnContrastSliderMove(wxCommandEvent &WXUNUSED(e)) 
{
	int raw = (int)KFrame::sliders[CONTRAST]->GetValue();
	float contrast_value = ((float)raw/100)*3;
	std::cout << "contrast slider: " << contrast_value << std::endl;
}

// Fired when the contrast slider is adjusted.
void KFrame::OnBrightnessSliderMove(wxCommandEvent &WXUNUSED(e)) 
{
	float brightness_value = (float)KFrame::sliders[BRIGHTNESS]->GetValue();
	std::cout << "brightness slider: " << brightness_value << std::endl;
}