#include "stdafx.h"
#include "Properties/LightStatesProperty.h"
#include <WindowsX.h>

LightStatesProperty::LightStatesProperty(const VectorProtected<ISelect> *pvsel) : BasePropertyDialog(IDD_PROPLIGHT_STATE, pvsel)
{
    m_stateList.push_back("Off");
    m_stateList.push_back("On");
    m_stateList.push_back("Blinking");
    m_blinkPatternEdit.SetDialog(this);
    m_blinkIntervalEdit.SetDialog(this);
    m_stateCombo.SetDialog(this);
}

void LightStatesProperty::UpdateVisuals(const int dispid/*=-1*/)
{
    for (int i = 0; i < m_pvsel->size(); i++)
    {
        if ((m_pvsel->ElementAt(i) == nullptr) || (m_pvsel->ElementAt(i)->GetItemType() != eItemLight))
            continue;
        const Light * const light = (Light *)m_pvsel->ElementAt(i);
        if (dispid == DISPID_Light_State || dispid == -1)
            PropertyDialog::UpdateComboBox(m_stateList, m_stateCombo, m_stateList[(int)light->getLightState()]);
        if (dispid == IDC_BLINK_PATTERN_EDIT || dispid == -1)
            m_blinkPatternEdit.SetWindowText(light->m_rgblinkpattern.c_str());
        if (dispid == DISPID_Light_BlinkInterval || dispid == -1)
            PropertyDialog::SetIntTextbox(m_blinkIntervalEdit, light->m_blinkinterval);
        //only show the first element on multi-select
        break;
    }
}

void LightStatesProperty::UpdateProperties(const int dispid)
{
    for (int i = 0; i < m_pvsel->size(); i++)
    {
        if ((m_pvsel->ElementAt(i) == nullptr) || (m_pvsel->ElementAt(i)->GetItemType() != eItemLight))
            continue;
        Light * const light = (Light *)m_pvsel->ElementAt(i);
        switch (dispid)
        {
            case DISPID_Light_State:
                CHECK_UPDATE_COMBO_VALUE_SETTER(light->setLightState, light->getLightState, (LightState)(PropertyDialog::GetComboBoxIndex(m_stateCombo, m_stateList)), light);
                break;
            case IDC_BLINK_PATTERN_EDIT:
            {
                const string pattern = m_blinkPatternEdit.GetWindowText();
                if (pattern != light->m_rgblinkpattern)
                {
                    PropertyDialog::StartUndo(light);
                    light->m_rgblinkpattern = pattern;
                    PropertyDialog::EndUndo(light);
                }
                break;
            }
            case DISPID_Light_BlinkInterval:
                CHECK_UPDATE_ITEM(light->m_blinkinterval, PropertyDialog::GetIntTextbox(m_blinkIntervalEdit), light);
                break;
            default:
                break;
        }
    }
    UpdateVisuals(dispid);
}

BOOL LightStatesProperty::OnInitDialog()
{
    m_stateCombo.AttachItem(DISPID_Light_State);
    m_blinkPatternEdit.AttachItem(IDC_BLINK_PATTERN_EDIT);
    m_blinkIntervalEdit.AttachItem(DISPID_Light_BlinkInterval);
    UpdateVisuals();
    m_stateCombo.SetDroppedWidth(20);

    m_resizer.Initialize(*this, CRect(0, 0, 0, 0));
    m_resizer.AddChild(GetDlgItem(IDC_STATIC1), topleft, 0);
    m_resizer.AddChild(GetDlgItem(IDC_STATIC2), topleft, 0);
    m_resizer.AddChild(GetDlgItem(IDC_STATIC3), topleft, 0);
    m_resizer.AddChild(m_stateCombo, topleft, RD_STRETCH_WIDTH);
    m_resizer.AddChild(m_blinkPatternEdit, topleft, RD_STRETCH_WIDTH );
    m_resizer.AddChild(m_blinkIntervalEdit, topleft, RD_STRETCH_WIDTH);

    return TRUE;
}

INT_PTR LightStatesProperty::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   m_resizer.HandleMessage(uMsg, wParam, lParam);
   return DialogProcDefault(uMsg, wParam, lParam);
}
