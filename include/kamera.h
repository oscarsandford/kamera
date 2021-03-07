// This is the header for kamera main, includes "back-end" functions  
// and definitions from the top-level header kutils.
#include <kutils.h>

// Text for wx labels. Add strings here when adding extra sliders.
char KMR_slider_labels[SLIDER_COUNT][20] = {
	"Contrast", "Brightness"
};

class KFrame : public wxFrame
{
	public:
		KFrame(const wxString &title, const wxPoint &, const wxSize &);
		wxSlider *sliders[SLIDER_COUNT];
		float slider_values[SLIDER_COUNT];
	private:
		void OnImport(wxCommandEvent &);
		void OnExport(wxCommandEvent &);
		void OnExit(wxCommandEvent &);
		void OnSliderMove(wxCommandEvent &);
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