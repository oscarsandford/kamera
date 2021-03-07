#include <kamera.h>
// Other wxWidgets libraries required for import/export GUI.
#include <wx/filedlg.h>
#include <wx/wfstream.h>
// Need this for strlen and strstr.
#include <cstring>

// Main app class definition.
class Kamera : public wxApp
{
	public:
		virtual bool OnInit();
};

// Program entry point. Event tables and implement (initialize).
wxBEGIN_EVENT_TABLE(KFrame, wxFrame)
	EVT_MENU(1, KFrame::OnImport)
	EVT_MENU(2, KFrame::OnExport)
	EVT_MENU(wxID_EXIT, KFrame::OnExit)
	EVT_SLIDER(wxID_ANY, KFrame::OnSliderMove)
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

// A silly fix, but I need to get the import method in KFrame to call a method 
// that belonds to KPreview, and above all initialize it in the KFrame constructor.
// I'll try to come back to this, or just redesign the two classes entirely.
KPreview *editor_preview;


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

	// Control slider for all the adjustment sliders
	wxBoxSizer *control_sizer = new wxBoxSizer(wxHORIZONTAL);
	
	// Arrays for adjustment slider's repective text and sizer
	wxStaticText *wxlabels[SLIDER_COUNT];
	wxBoxSizer *slider_sizers[SLIDER_COUNT];

	// Make vertical sliders for each adjustment that go from 1-100, label them with KMR labels defined in kamera.h
	for (int i = 0; i < SLIDER_COUNT; i++) 
	{
		KFrame::sliders[i] = new wxSlider(bottom_panel, wxID_ANY, 10, 1, 100, wxDefaultPosition, wxSize(100,100), wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
		wxlabels[i] = new wxStaticText(bottom_panel, wxID_ANY, KMR_slider_labels[i], wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	}

	// Add sliders and labels to control sizer
	for (int i = 0; i < SLIDER_COUNT; i++) 
	{
		slider_sizers[i] = new wxBoxSizer(wxVERTICAL);
		slider_sizers[i]->Add(wxlabels[i], 0, wxALIGN_CENTER_HORIZONTAL);
		slider_sizers[i]->Add(KFrame::sliders[i], 1, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxRIGHT, 10);
		control_sizer->Add(slider_sizers[i], 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
	}

	// Bind their slider event such taht OnSliderMove is triggered when slider is adjusted.
	for (int i = 0; i < SLIDER_COUNT; i++) 
	{
		KFrame::sliders[i]->Bind(wxEVT_SLIDER, &KFrame::OnSliderMove, this);
	}
	
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
	// Derive the C string of the path. Might not be the safest..
	char path[1024];
	strncpy(path, (const char *)importFileDialog.GetPath().mb_str(wxConvUTF8), 1023);

	// Call function to create the image Mat by path.
	editor_preview->img_mat = KMR_import(path);

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
	// Derive the C string of the path. Might not be the safest..
	char path[1024];
	strncpy(path, (const char *)exportFileDialog.GetPath().mb_str(wxConvUTF8), 1023);

	// Update the imported Mat object based on current slider values.
	editor_preview->img_mat = KMR_adjust(editor_preview->img_mat, slider_values);
	// Call function to export the image by path.
	KMR_export(editor_preview->img_mat, path);
}

// When any slider is adjusted, update the slider values.
void KFrame::OnSliderMove(wxCommandEvent &WXUNUSED(e)) 
{
	int contrast_raw = (int)KFrame::sliders[CONTRAST]->GetValue();
	slider_values[CONTRAST] = ((float)contrast_raw/100)*3;
	slider_values[BRIGHTNESS] = (float)KFrame::sliders[BRIGHTNESS]->GetValue();
	std::cout << "sliders (contrast, brightness): " << slider_values[CONTRAST] << " , " << slider_values[BRIGHTNESS] << std::endl;
}