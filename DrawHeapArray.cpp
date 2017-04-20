#include "wx/wx.h"
#include "wx/sizer.h"

#include "DrawPanel.h"
#include "ImageLoader.h"
#include "Circle.h"
#include "Line.h"
#include "HeapArray.h"
#include "CD.h"
#include "String.h"
using CSC2110::String;

#include <iostream>
using namespace std;

class MyApp: public wxApp
{
    bool OnInit();
 
    wxFrame* frame;
    DrawPanel* drawPane;
public:
 
};

IMPLEMENT_APP(MyApp)

//int main(int argc, char** argv)
bool MyApp::OnInit()
{
   ListArray<CD>* cds = CD::readCDs("cds.txt");
   int num_items = cds->size();
   cout << num_items << endl;

   ListArrayIterator<CD>* iter = cds->iterator();
   HeapArray<CD>* sh = new HeapArray<CD>(&CD::compare_items);
   while(iter->hasNext())
   {
      CD* cd = iter->next();
      sh->heapInsert(cd);
   }
   delete iter;
   delete cds;

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Heap Array!"), wxPoint(500,500), wxSize(1100,600));
 
    drawPane = new DrawPanel((wxFrame*) frame, sh);

    sizer->Add(drawPane, 1, wxEXPAND);
 
    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);
 
    frame->Show();
    return true;

   return 0;
}
