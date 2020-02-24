#include "StdAfx.h"
#include "resource.h"
#include <windowsx.h>

#define RECOMPUTEBUTTONCHECK WM_USER+100
#define RESIZE_FROM_EXPAND WM_USER+101

/*TBBUTTON const g_tbbuttonDebug[] = {
#ifdef _WIN64
{ 0, IDC_PLAY, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, {0, 0, 0, 0, 0, 0}, IDS_PLAY, 0 },
{ 1, IDC_PAUSE, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, {0, 0, 0, 0, 0, 0}, IDS_PAUSE, 1 },
{ 2, IDC_STEP, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, {0, 0, 0, 0, 0, 0}, IDS_STEP, 2 },
#else
{ 0, IDC_PLAY, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, {0, 0}, IDS_PLAY, 0 },
{ 1, IDC_PAUSE, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, {0, 0}, IDS_PAUSE, 1 },
{ 2, IDC_STEP, TBSTATE_ENABLED, TBSTYLE_CHECKGROUP, {0, 0}, IDS_STEP, 2 },
#endif
};*/

void AssignIconToButton(HWND hwnd, int controlid, int resourceid)
{
   HWND hwndButton = GetDlgItem(hwnd, controlid);
   HICON hicon = (HICON)LoadImage(g_hinst, MAKEINTRESOURCE(resourceid), IMAGE_ICON, 16, 16, 0);
   SendMessage(hwndButton, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon);
}

Light *GetLight(HWND hCombo)
{
   int idx_row;
   char strText[255] = { 0 };
   idx_row = ComboBox_GetCurSel(hCombo);
   ComboBox_GetLBText(hCombo, idx_row, strText);
   IEditable * const pedit = g_pplayer->m_ptable->GetElementByName(strText);
   if (pedit != NULL)
      return (Light*)pedit;

   return NULL;
}

void SetCheckButtonState(HWND hwndDlg, Light *plight)
{
   if (plight->m_d.m_state == LightStateOn)
   {
      Button_SetCheck(GetDlgItem(hwndDlg, IDC_DBG_LIGHT_ON_CHECK), BST_CHECKED);
      Button_SetCheck(GetDlgItem(hwndDlg, IDC_DBG_LIGHT_OFF_CHECK), BST_UNCHECKED);
      Button_SetCheck(GetDlgItem(hwndDlg, IDC_DBG_LIGHT_BLINKING_CHECK), BST_UNCHECKED);
   }
   else if (plight->m_d.m_state == LightStateOff)
   {
      Button_SetCheck(GetDlgItem(hwndDlg, IDC_DBG_LIGHT_ON_CHECK), BST_UNCHECKED);
      Button_SetCheck(GetDlgItem(hwndDlg, IDC_DBG_LIGHT_OFF_CHECK), BST_CHECKED);
      Button_SetCheck(GetDlgItem(hwndDlg, IDC_DBG_LIGHT_BLINKING_CHECK), BST_UNCHECKED);
   }
   else if (plight->m_d.m_state == LightStateBlinking)
   {
      Button_SetCheck(GetDlgItem(hwndDlg, IDC_DBG_LIGHT_ON_CHECK), BST_UNCHECKED);
      Button_SetCheck(GetDlgItem(hwndDlg, IDC_DBG_LIGHT_OFF_CHECK), BST_UNCHECKED);
      Button_SetCheck(GetDlgItem(hwndDlg, IDC_DBG_LIGHT_BLINKING_CHECK), BST_CHECKED);
   }
}
INT_PTR CALLBACK MaterialDebuggerProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   PinTable * const ptable = g_pplayer->m_ptable;
   HWND hCombo = GetDlgItem(hwndDlg, IDC_DBG_MATERIALCOMBO);
   switch (uMsg)
   {
   case WM_INITDIALOG:
   {
      vector<string> matNames;
      SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lParam);
      for (size_t i = 0; i < ptable->m_materials.size(); i++)
      {
         matNames.push_back(ptable->m_materials[i]->m_szName);
      }
      std::sort(matNames.begin(), matNames.end());
      for (size_t i = 0; i < matNames.size(); i++)
         ComboBox_AddString(hCombo, matNames[i].c_str());

      ShowWindow(g_pplayer->m_hwndDebugger, SW_HIDE);
      ComboBox_SetCurSel(hCombo, 0);
      SendMessage(hwndDlg, WM_COMMAND, MAKEWPARAM(IDC_DBG_MATERIALCOMBO, CBN_SELCHANGE), 0);
      return TRUE;
   }
   case WM_CLOSE:
   {
      ShowWindow(g_pplayer->m_hwndDebugger, SW_SHOW);
      EndDialog(hwndDlg, FALSE);
      break;
   }
   case WM_ACTIVATE:
   {
      g_pplayer->m_debugWindowActive = (wParam != WA_INACTIVE);
      g_pplayer->RecomputePauseState();
      g_pplayer->RecomputePseudoPauseState();
      break;
   }
   case WM_COMMAND:
   {
      switch (HIWORD(wParam))
      {
      case BN_CLICKED:
      {
         switch (LOWORD(wParam))
         {
         case IDOK:
         {
            int idx_row;
            char strText[255] = { 0 };
            idx_row = ComboBox_GetCurSel(hCombo);
            ComboBox_GetLBText(hCombo, idx_row, strText);

            Material * const pMat = ptable->GetMaterial(strText);
            if (pMat != &g_pvp->m_dummyMaterial)
            {
               char value[256];
               GetDlgItemText(hwndDlg, IDC_DBG_MATERIAL_BASE_WRAP_EDIT, value, 31);
               pMat->m_fWrapLighting = saturate(sz2f(value));
               GetDlgItemText(hwndDlg, IDC_DBG_MATERIAL_SHININESS_EDIT, value, 31);
               pMat->m_fRoughness = saturate(sz2f(value));
               GetDlgItemText(hwndDlg, IDC_DBG_MATERIAL_GLOSSY_IMGLERP_EDIT, value, 31);
               pMat->m_fGlossyImageLerp = saturate(sz2f(value));
               GetDlgItemText(hwndDlg, IDC_DBG_MATERIAL_THICKNESS_EDIT, value, 31);
               pMat->m_fThickness = saturate(sz2f(value));
               GetDlgItemText(hwndDlg, IDC_DBG_MATERIAL_EDGE_EDIT, value, 31);
               pMat->m_fEdge = saturate(sz2f(value));
               GetDlgItemText(hwndDlg, IDC_DBG_MATERIAL_OPACITY_AMOUNT_EDIT, value, 31);
               pMat->m_fOpacity = saturate(sz2f(value));
               GetDlgItemText(hwndDlg, DBG_MATERIAL_OPACITY_EDGE_EDIT, value, 31);
               pMat->m_fEdgeAlpha = saturate(sz2f(value));
               size_t checked = SendDlgItemMessage(hwndDlg, IDC_DBG_METAL_MATERIAL_CHECK, BM_GETCHECK, 0, 0);
               pMat->m_bIsMetal = (checked == 1);
               checked = SendDlgItemMessage(hwndDlg, IDC_DBG_MATERIAL_OPACITY_ACTIVE_CHECK, BM_GETCHECK, 0, 0);
               pMat->m_bOpacityActive = (checked == 1);
               ptable->AddDbgMaterial(pMat);
            }
            break;
         }
         case IDCANCEL:
         {

            ShowWindow(g_pplayer->m_hwndDebugger, SW_SHOW);
            EndDialog(hwndDlg, FALSE);
            return TRUE;
         }
         }
         break;
      }
      }
      switch (LOWORD(wParam))
      {
      case IDC_DBG_MATERIAL_OPACITY_ACTIVE_CHECK:
      case IDC_DBG_METAL_MATERIAL_CHECK:
      {
         SendMessage(hwndDlg, WM_COMMAND, MAKEWPARAM(IDOK, BN_CLICKED), 0);
         break;
      }
      case IDC_DBG_MATERIALCOMBO:
      {
         switch (HIWORD(wParam))
         {
         case CBN_SELCHANGE:
         {
            int idx_row;
            char strText[255] = { 0 };
            idx_row = ComboBox_GetCurSel(hCombo);
            ComboBox_GetLBText(hCombo, idx_row, strText);

            Material *const pMat = ptable->GetMaterial(strText);
            if (pMat != &g_pvp->m_dummyMaterial)
            {
               char value[256];
               f2sz(pMat->m_fWrapLighting, value);
               SetDlgItemText(hwndDlg, IDC_DBG_MATERIAL_BASE_WRAP_EDIT, value);
               f2sz(pMat->m_fRoughness, value);
               SetDlgItemText(hwndDlg, IDC_DBG_MATERIAL_SHININESS_EDIT, value);
               f2sz(pMat->m_fGlossyImageLerp, value);
               SetDlgItemText(hwndDlg, IDC_DBG_MATERIAL_GLOSSY_IMGLERP_EDIT, value);
               f2sz(pMat->m_fThickness, value);
               SetDlgItemText(hwndDlg, IDC_DBG_MATERIAL_THICKNESS_EDIT, value);
               f2sz(pMat->m_fEdge, value);
               SetDlgItemText(hwndDlg, IDC_DBG_MATERIAL_EDGE_EDIT, value);
               f2sz(pMat->m_fOpacity, value);
               SetDlgItemText(hwndDlg, IDC_DBG_MATERIAL_OPACITY_AMOUNT_EDIT, value);
               f2sz(pMat->m_fEdgeAlpha, value);
               SetDlgItemText(hwndDlg, DBG_MATERIAL_OPACITY_EDGE_EDIT, value);
               SendMessage(GetDlgItem(hwndDlg, IDC_DBG_METAL_MATERIAL_CHECK), BM_SETCHECK, pMat->m_bIsMetal ? BST_CHECKED : BST_UNCHECKED, 0);
               SendMessage(GetDlgItem(hwndDlg, IDC_DBG_MATERIAL_OPACITY_ACTIVE_CHECK), BM_SETCHECK, pMat->m_bOpacityActive ? BST_CHECKED : BST_UNCHECKED, 0);
            }
            break;
         }
         }
         break;
      }
      }
      break;
   }
   }
   return FALSE;
}

INT_PTR CALLBACK LightDebuggerProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   PinTable * const ptable = g_pplayer->m_ptable;
   HWND hCombo = GetDlgItem(hwndDlg, IDC_LIGHTSCOMBO);

   switch (uMsg)
   {
   case WM_INITDIALOG:
   {
      vector<string> lightNames;
      SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lParam);
      for (size_t i = 0; i < ptable->m_vedit.size(); i++)
      {
         IEditable * const pedit = ptable->m_vedit[i];
         if (pedit->GetItemType() == eItemLight)
         {
            lightNames.push_back(ptable->GetElementName(pedit));
         }
      }
      std::sort(lightNames.begin(), lightNames.end());
      for (size_t i = 0; i < lightNames.size();i++)
         ComboBox_AddString(hCombo, lightNames[i].c_str());

      ShowWindow(g_pplayer->m_hwndDebugger, SW_HIDE);
      ComboBox_SetCurSel(hCombo, 0);
      SendMessage(hwndDlg, WM_COMMAND, MAKEWPARAM(IDC_LIGHTSCOMBO, CBN_SELCHANGE), 0);
      return TRUE;
   }
   case WM_CLOSE:
   {
      ShowWindow(g_pplayer->m_hwndDebugger, SW_SHOW);
      EndDialog(hwndDlg, FALSE);
      break;
   }
   case WM_ACTIVATE:
   {
      g_pplayer->m_debugWindowActive = (wParam != WA_INACTIVE);
      g_pplayer->RecomputePauseState();
      g_pplayer->RecomputePseudoPauseState();
      break;
   }
   case WM_COMMAND:
   {
      switch (HIWORD(wParam))
      {
      case BN_CLICKED:
      {
         switch (LOWORD(wParam))
         {
         case IDOK:
         {
            Light *plight = GetLight(hCombo);
            if (plight != NULL)
            {
               char value[256];
               float fv;

               GetDlgItemText(hwndDlg, IDC_DBG_LIGHT_FALLOFF, value, 31);
               fv = sz2f(value);
               plight->put_Falloff(fv);
               GetDlgItemText(hwndDlg, IDC_DBG_LIGHT_FALLOFF_POWER, value, 31);
               fv = sz2f(value);
               plight->put_FalloffPower(fv);
               GetDlgItemText(hwndDlg, IDC_DBG_LIGHT_INTENSITY, value, 31);
               fv = sz2f(value);
               plight->put_Intensity(fv);
               GetDlgItemText(hwndDlg, IDC_DBG_BULB_MODULATE_VS_ADD, value, 31);
               fv = sz2f(value);
               plight->m_d.m_modulate_vs_add = fv;
               GetDlgItemText(hwndDlg, IDC_DBG_TRANSMISSION_SCALE, value, 31);
               fv = sz2f(value);
               plight->m_d.m_transmissionScale = fv;
               GetDlgItemText(hwndDlg, IDC_DBG_LIGHT_FADE_UP_EDIT, value, 31);
               fv = sz2f(value);
               plight->put_FadeSpeedUp(fv);
               GetDlgItemText(hwndDlg, IDC_DBG_LIGHT_FADE_DOWN_EDIT, value, 31);
               fv = sz2f(value);
               plight->put_FadeSpeedDown(fv);
               ptable->AddDbgLight(plight);
            };
            break;
         }
         case IDCANCEL:
         {

            ShowWindow(g_pplayer->m_hwndDebugger, SW_SHOW);
            EndDialog(hwndDlg, FALSE);
            return TRUE;
         }
         case IDC_DBG_LIGHT_ON_CHECK:
         {
            Light *plight = GetLight(hCombo);
            if (plight != NULL)
            {
               plight->put_State(LightStateOn);
               SetCheckButtonState(hwndDlg, plight);
               ptable->AddDbgLight(plight);
            }
            break;
         }
         case IDC_DBG_LIGHT_OFF_CHECK:
         {
            Light *plight = GetLight(hCombo);
            if (plight != NULL)
            {
               plight->put_State(LightStateOff);
               SetCheckButtonState(hwndDlg, plight);
               ptable->AddDbgLight(plight);
            }
            break;
         }
         case IDC_DBG_LIGHT_BLINKING_CHECK:
         {
            Light *plight = GetLight(hCombo);
            if (plight != NULL)
            {
               plight->put_State(LightStateBlinking);
               SetCheckButtonState(hwndDlg, plight);
               ptable->AddDbgLight(plight);
            }
            break;
         }
         }
         break;
      }
      }
      switch (LOWORD(wParam))
      {
      case IDC_LIGHTSCOMBO:
      {
         switch (HIWORD(wParam))
         {
         case CBN_SELCHANGE:
         {
            Light *plight = GetLight(hCombo);
            if (plight != NULL)
            {
               char value[256];
               float v;
               plight->get_Falloff(&v);
               f2sz(v, value);
               SetDlgItemText(hwndDlg, IDC_DBG_LIGHT_FALLOFF, value);
               plight->get_FalloffPower(&v);
               f2sz(v, value);
               SetDlgItemText(hwndDlg, IDC_DBG_LIGHT_FALLOFF_POWER, value);
               plight->get_Intensity(&v);
               f2sz(v, value);
               SetDlgItemText(hwndDlg, IDC_DBG_LIGHT_INTENSITY, value);
               v = plight->m_d.m_modulate_vs_add;
               f2sz(v, value);
               SetDlgItemText(hwndDlg, IDC_DBG_BULB_MODULATE_VS_ADD, value);
               v = plight->m_d.m_transmissionScale;
               f2sz(v, value);
               SetDlgItemText(hwndDlg, IDC_DBG_TRANSMISSION_SCALE, value);
               plight->get_FadeSpeedUp(&v);
               f2sz(v, value);
               SetDlgItemText(hwndDlg, IDC_DBG_LIGHT_FADE_UP_EDIT, value);
               plight->get_FadeSpeedDown(&v);
               f2sz(v, value);
               SetDlgItemText(hwndDlg, IDC_DBG_LIGHT_FADE_DOWN_EDIT, value);
               SetCheckButtonState(hwndDlg, plight);
            };
            break;
         }
         }
      }
      }
      break;
   }
   }
   return FALSE;
}

INT_PTR CALLBACK DebuggerProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
   case WM_INITDIALOG:
   {
      RECT rcDialog;
      RECT rcMain;
      GetWindowRect(GetParent(hwndDlg), &rcMain);
      GetWindowRect(hwndDlg, &rcDialog);

      SetWindowPos(hwndDlg, NULL,
         (rcMain.right + rcMain.left) / 2 - (rcDialog.right - rcDialog.left) / 2,
         (rcMain.bottom + rcMain.top) / 2 - (rcDialog.bottom - rcDialog.top) / 2,
         0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE/* | SWP_NOMOVE*/);

      AssignIconToButton(hwndDlg, IDC_PLAY, IDI_PLAY);
      AssignIconToButton(hwndDlg, IDC_PAUSE, IDI_PAUSE);
      AssignIconToButton(hwndDlg, IDC_STEP, IDI_STEP);

      SendMessage(hwndDlg, RECOMPUTEBUTTONCHECK, 0, 0);

      RECT rcEditSize;
      HWND hwndEditSize = GetDlgItem(hwndDlg, IDC_EDITSIZE);
      GetWindowRect(hwndEditSize, &rcEditSize);

      ScreenToClient(hwndDlg, (POINT *)&rcEditSize);
      ScreenToClient(hwndDlg, &((POINT *)&rcEditSize)[1]);

      g_pplayer->m_hwndDebugOutput = CreateWindowEx(0, "Scintilla", "",
         WS_CHILD | ES_NOHIDESEL | WS_VISIBLE | ES_SUNKEN | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | ES_WANTRETURN | WS_BORDER,
         rcEditSize.left, rcEditSize.top, rcEditSize.right - rcEditSize.left, rcEditSize.bottom - rcEditSize.top, hwndDlg, NULL, g_hinst, 0);

      SendMessage(g_pplayer->m_hwndDebugOutput, SCI_STYLESETSIZE, 32, 10);
      SendMessage(g_pplayer->m_hwndDebugOutput, SCI_STYLESETFONT, 32, (LPARAM)"Courier");

      SendMessage(g_pplayer->m_hwndDebugOutput, SCI_SETMARGINWIDTHN, 1, 0);

      SendMessage(g_pplayer->m_hwndDebugOutput, SCI_SETTABWIDTH, 4, 0);

      SendDlgItemMessage(hwndDlg, IDC_BALL_THROWING, BM_SETCHECK, g_pplayer->m_throwBalls ? BST_CHECKED : BST_UNCHECKED, 0);
      SendDlgItemMessage(hwndDlg, IDC_BALL_CONTROL, BM_SETCHECK, g_pplayer->m_ballControl ? BST_CHECKED : BST_UNCHECKED, 0);

      SetDlgItemInt(hwndDlg, IDC_THROW_BALL_SIZE_EDIT2, g_pplayer->m_debugBallSize, FALSE);
      char textBuf[256] = { 0 };
      f2sz(g_pplayer->m_debugBallMass, textBuf);
      SetDlgItemText(hwndDlg, IDC_THROW_BALL_MASS_EDIT2, textBuf);

      SendMessage(hwndDlg, RESIZE_FROM_EXPAND, 0, 0);

      return TRUE;
   }

   case WM_NOTIFY:
   {
      //int idCtrl = (int) wParam;
      const NMHDR *const pnmh = (LPNMHDR)lParam;
      //HWND hwndRE = pnmh->hwndFrom;
      const int code = pnmh->code;

      switch (code)
      {
      case SCN_CHARADDED:
      {
         const SCNotification * const pscnmh = (SCNotification *)lParam;
         if (pscnmh->ch == '\n')
         {
            SendMessage(pnmh->hwndFrom, SCI_DELETEBACK, 0, 0);

            const size_t curpos = SendMessage(pnmh->hwndFrom, SCI_GETCURRENTPOS, 0, 0);
            const size_t line = SendMessage(pnmh->hwndFrom, SCI_LINEFROMPOSITION, curpos, 0);
            const size_t lineStart = SendMessage(pnmh->hwndFrom, SCI_POSITIONFROMLINE, line, 0);
            const size_t lineEnd = SendMessage(pnmh->hwndFrom, SCI_GETLINEENDPOSITION, line, 0);

            char * const szText = new char[lineEnd - lineStart + 1];
            TextRange tr;
            tr.chrg.cpMin = (Sci_PositionCR)lineStart;
            tr.chrg.cpMax = (Sci_PositionCR)lineEnd;
            tr.lpstrText = szText;
            SendMessage(pnmh->hwndFrom, SCI_GETTEXTRANGE, 0, (LPARAM)&tr);

            const size_t maxlines = SendMessage(pnmh->hwndFrom, SCI_GETLINECOUNT, 0, 0);

            if (maxlines == line + 1)
            {
               // need to add a new line to the end
               SendMessage(pnmh->hwndFrom, SCI_DOCUMENTEND, 0, 0);
               SendMessage(pnmh->hwndFrom, SCI_ADDTEXT, lstrlen("\n"), (LPARAM)"\n");
            }
            else
            {
               const size_t pos = SendMessage(pnmh->hwndFrom, SCI_POSITIONFROMLINE, line + 1, 0);
               SendMessage(pnmh->hwndFrom, SCI_SETCURRENTPOS, pos, 0);
            }

            g_pplayer->m_ptable->m_pcv->EvaluateScriptStatement(szText);
            delete[] szText;
         }
         break;
      }
      }//switch (code)
      break;
   }
   case RECOMPUTEBUTTONCHECK:
   {
      int PlayDown = BST_UNCHECKED;
      int PauseDown = BST_UNCHECKED;
      int StepDown = BST_UNCHECKED;

      if (g_pplayer->m_userDebugPaused)
      {
         PauseDown = BST_CHECKED;
      }
#ifdef STEPPING
      else if (g_pplayer->m_pauseTimeTarget > 0)
      {
         StepDown = BST_CHECKED;
      }
#endif
      else
      {
         PlayDown = BST_CHECKED;
      }

      const HWND hwndTBParent = GetDlgItem(hwndDlg, IDC_TOOLBARSIZE);
      const HWND hwndToolbar = GetWindow(hwndTBParent, GW_CHILD);

      SendDlgItemMessage(hwndDlg, IDC_PLAY, BM_SETCHECK, PlayDown, 0);
      SendDlgItemMessage(hwndDlg, IDC_PAUSE, BM_SETCHECK, PauseDown, 0);
      SendDlgItemMessage(hwndDlg, IDC_STEP, BM_SETCHECK, StepDown, 0);
      SendMessage(hwndToolbar, TB_CHECKBUTTON, IDC_PLAY, PlayDown);
      SendMessage(hwndToolbar, TB_CHECKBUTTON, IDC_PAUSE, PauseDown);
      SendMessage(hwndToolbar, TB_CHECKBUTTON, IDC_STEP, StepDown);
      break;
   }

   case WM_CLOSE:
   {
#ifdef STEPPING
      g_pplayer->m_pauseTimeTarget = 0;
#endif
      g_pplayer->m_userDebugPaused = false;
      g_pplayer->RecomputePseudoPauseState();
      g_pplayer->m_debugBallSize = GetDlgItemInt(hwndDlg, IDC_THROW_BALL_SIZE_EDIT2, NULL, FALSE);
      char textBuf[256] = { 0 };
      float fv;
      GetDlgItemText(hwndDlg, IDC_THROW_BALL_MASS_EDIT2, textBuf, 255);
      fv = sz2f(textBuf);
      g_pplayer->m_debugBallMass = fv;
      g_pplayer->m_debugMode = false;
      g_pplayer->m_showDebugger = false;
      ShowWindow(hwndDlg, SW_HIDE);
      break;
   }

   case WM_ACTIVATE:
   {
      g_pplayer->m_debugWindowActive = (wParam != WA_INACTIVE);
      g_pplayer->RecomputePauseState();
      g_pplayer->RecomputePseudoPauseState();
      break;
   }

   case RESIZE_FROM_EXPAND:
   {
      const size_t state = SendDlgItemMessage(hwndDlg, IDC_EXPAND, BM_GETCHECK, 0, 0);
      int mult;
      if (state == BST_CHECKED)
      {
         mult = 1;
         SetWindowText(GetDlgItem(hwndDlg, IDC_EXPAND), "< Editor");
      }
      else
      {
         mult = -1;
         SetWindowText(GetDlgItem(hwndDlg, IDC_EXPAND), "> Editor");
      }

      RECT rcSizer1;
      RECT rcSizer2;
      GetWindowRect(GetDlgItem(hwndDlg, IDC_GUIDE1), &rcSizer1);
      GetWindowRect(GetDlgItem(hwndDlg, IDC_GUIDE2), &rcSizer2);

      const int diffx = rcSizer2.right - rcSizer1.right;
      const int diffy = rcSizer2.bottom - rcSizer1.bottom;

      RECT rcDialog;
      GetWindowRect(hwndDlg, &rcDialog);

      SetWindowPos(hwndDlg, NULL,
         rcDialog.left,
         rcDialog.top,
         rcDialog.right - rcDialog.left + diffx * mult, rcDialog.bottom - rcDialog.top + diffy * mult, SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
      break;
   }

   case WM_COMMAND:
   {
      switch (HIWORD(wParam))
      {
      case BN_CLICKED:
      {
         switch (LOWORD(wParam))
         {
         case IDC_PLAY:
         {
#ifdef STEPPING
            g_pplayer->m_pauseTimeTarget = 0;
#endif
            g_pplayer->m_userDebugPaused = false;
            g_pplayer->RecomputePseudoPauseState();
            SendMessage(hwndDlg, RECOMPUTEBUTTONCHECK, 0, 0);
            break;
         }
         case IDC_PAUSE:
         {
#ifdef STEPPING
            g_pplayer->m_pauseTimeTarget = 0;
#endif
            g_pplayer->m_userDebugPaused = true;
            g_pplayer->RecomputePseudoPauseState();
            SendMessage(hwndDlg, RECOMPUTEBUTTONCHECK, 0, 0);
            break;
         }
         case IDC_STEP:
         {
#ifdef STEPPING
            const int ms = GetDlgItemInt(hwndDlg, IDC_STEPAMOUNT, NULL, FALSE);
            g_pplayer->m_pauseTimeTarget = g_pplayer->m_time_msec + ms;
#endif
            g_pplayer->m_userDebugPaused = false;
            g_pplayer->RecomputePseudoPauseState();
            SendMessage(hwndDlg, RECOMPUTEBUTTONCHECK, 0, 0);
            break;
         }
         case IDC_EXPAND:
         {
            SendMessage(hwndDlg, RESIZE_FROM_EXPAND, 0, 0);
            break;
         }
         case IDC_BALL_THROWING:
         {
            HWND hwndControl = GetDlgItem(hwndDlg, IDC_BALL_THROWING);
            size_t checked = SendMessage(hwndControl, BM_GETCHECK, 0, 0);
            g_pplayer->m_throwBalls = !!checked;
            break;
         }
         case IDC_BALL_CONTROL:
         {
            HWND hwndControl = GetDlgItem(hwndDlg, IDC_BALL_CONTROL);
            size_t checked = SendMessage(hwndControl, BM_GETCHECK, 0, 0);
            g_pplayer->m_ballControl = !!checked;
            break;
         }

         case IDC_DBGLIGHTSBUTTON:
         {
            g_pplayer->m_hwndLightDebugger = CreateDialogParam(g_hinst, MAKEINTRESOURCE(IDD_DBGLIGHTDIALOG), hwndDlg, LightDebuggerProc, NULL);
            break;
         }
         case IDC_DBG_MATERIALS_BUTTON:
         {
            g_pplayer->m_hwndLightDebugger = CreateDialogParam(g_hinst, MAKEINTRESOURCE(IDD_DBGMATERIALDIALOG), hwndDlg, MaterialDebuggerProc, NULL);
            break;
         }
         }
         break;
      }//case BN_CLICKED:
      }//switch (HIWORD(wParam))
      break;
   }//case WM_COMMAND:
   default:
   {
      switch (LOWORD(wParam))
      {
      case IDC_THROW_BALL_SIZE_EDIT2:
      {
         g_pplayer->m_debugBallSize = GetDlgItemInt(hwndDlg, IDC_THROW_BALL_SIZE_EDIT2, NULL, FALSE);
         break;
      }
      }
      break;
   }
   }

   return FALSE;
}
