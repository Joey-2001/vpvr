#include "stdafx.h"
#include "Properties/TableVisualsProperty.h"
#include <WindowsX.h>

TableVisualsProperty::TableVisualsProperty(const VectorProtected<ISelect> *pvsel) : BasePropertyDialog(IDD_PROPTABLE_VISUALS, pvsel)
{
    m_reflectionStrengthEdit.SetDialog(this);
    m_ballReflectPlayfieldEdit.SetDialog(this);
    m_ballDefaultBulbIntensScaleEdit.SetDialog(this);
    m_imageCombo.SetDialog(this);
    m_materialCombo.SetDialog(this);
    m_ballImageCombo.SetDialog(this);
    m_ballDecalCombo.SetDialog(this);
}

void TableVisualsProperty::UpdateVisuals(const int dispid/*=-1*/)
{
    CComObject<PinTable> * const table = g_pvp->GetActiveTable();
    if (table == nullptr)
        return;

    if (dispid == DISPID_Image || dispid == -1)
        PropertyDialog::UpdateTextureComboBox(table->GetImageList(), m_imageCombo, table->m_image);
    if (dispid == IDC_MATERIAL_COMBO || dispid == -1)
        PropertyDialog::UpdateMaterialComboBox(table->GetMaterialList(), m_materialCombo, table->m_playfieldMaterial);
    if (dispid == IDC_REFLECT_ELEMENTS_CHECK || dispid == -1)
        PropertyDialog::SetCheckboxState(m_hReflectElementsCheck, table->m_reflectElementsOnPlayfield);
    if (dispid == IDC_REFLECTION_PLAYFIELD || dispid == -1)
        PropertyDialog::SetIntTextbox(m_reflectionStrengthEdit, table->GetPlayfieldReflectionStrength());
    if (dispid == 1505 || dispid == -1)
        PropertyDialog::UpdateTextureComboBox(table->GetImageList(), m_ballImageCombo, table->m_ballImage);
    if (dispid == IDC_BALL_DECAL_MODE || dispid == -1)
        PropertyDialog::SetCheckboxState(m_hLogoModeCheck, table->m_BallDecalMode);
    if (dispid == 1508 || dispid == -1)
        PropertyDialog::UpdateTextureComboBox(table->GetImageList(), m_ballDecalCombo, table->m_ballImageDecal);
    if (dispid == IDC_BALLPLAYFIELD_REFLECTION || dispid == -1)
        PropertyDialog::SetFloatTextbox(m_ballReflectPlayfieldEdit, table->m_ballPlayfieldReflectionStrength);
    if (dispid == IDC_BULBINTENSITYSCALE || dispid == -1)
        PropertyDialog::SetFloatTextbox(m_ballDefaultBulbIntensScaleEdit, table->m_defaultBulbIntensityScaleOnBall);
}

void TableVisualsProperty::UpdateProperties(const int dispid)
{
    CComObject<PinTable> * const table = g_pvp->GetActiveTable();
    if (table == nullptr)
        return;

    switch (dispid)
    {
        case DISPID_Image:
            CHECK_UPDATE_COMBO_TEXT_STRING(table->m_image, m_imageCombo, table);
            break;
        case IDC_MATERIAL_COMBO:
            CHECK_UPDATE_COMBO_TEXT_STRING(table->m_playfieldMaterial, m_materialCombo, table);
            break;
        case IDC_REFLECT_ELEMENTS_CHECK:
            CHECK_UPDATE_ITEM(table->m_reflectElementsOnPlayfield, PropertyDialog::GetCheckboxState(m_hReflectElementsCheck), table);
            break;
        case IDC_REFLECTION_PLAYFIELD:
            CHECK_UPDATE_VALUE_SETTER(table->SetPlayfieldReflectionStrength, table->GetPlayfieldReflectionStrength, PropertyDialog::GetIntTextbox, m_reflectionStrengthEdit, table);
            break;
        case IDC_BALL_DECAL_MODE:
            CHECK_UPDATE_ITEM(table->m_BallDecalMode, PropertyDialog::GetCheckboxState(m_hLogoModeCheck), table);
            break;
        case 1505:
            CHECK_UPDATE_COMBO_TEXT_STRING(table->m_ballImage, m_ballImageCombo, table);
            break;
        case 1508:
            CHECK_UPDATE_COMBO_TEXT_STRING(table->m_ballImageDecal, m_ballDecalCombo, table);
            break;
        case IDC_BALLPLAYFIELD_REFLECTION:
            CHECK_UPDATE_ITEM(table->m_ballPlayfieldReflectionStrength, PropertyDialog::GetFloatTextbox(m_ballReflectPlayfieldEdit), table);
            break;
        case IDC_BULBINTENSITYSCALE:
            CHECK_UPDATE_ITEM(table->m_defaultBulbIntensityScaleOnBall, PropertyDialog::GetFloatTextbox(m_ballDefaultBulbIntensScaleEdit), table);
            break;
        default:
            break;
    }
    UpdateVisuals(dispid);
}

BOOL TableVisualsProperty::OnInitDialog()
{
    m_imageCombo.AttachItem(DISPID_Image);
    m_materialCombo.AttachItem(IDC_MATERIAL_COMBO);
    m_ballImageCombo.AttachItem(1505);
    m_ballDecalCombo.AttachItem(1508);
    m_hReflectElementsCheck = ::GetDlgItem(GetHwnd(), IDC_REFLECT_ELEMENTS_CHECK);
    m_reflectionStrengthEdit.AttachItem(IDC_REFLECTION_PLAYFIELD);
    m_hLogoModeCheck = ::GetDlgItem(GetHwnd(), IDC_BALL_DECAL_MODE);
    m_ballReflectPlayfieldEdit.AttachItem(IDC_BALLPLAYFIELD_REFLECTION);
    m_ballDefaultBulbIntensScaleEdit.AttachItem(IDC_BULBINTENSITYSCALE);

    UpdateVisuals(); 
    
    m_resizer.Initialize(*this, CRect(0, 0, 0, 0));
    m_resizer.AddChild(GetDlgItem(IDC_STATIC1), topleft, 0);
    m_resizer.AddChild(GetDlgItem(IDC_STATIC2), topleft, 0);
    m_resizer.AddChild(GetDlgItem(IDC_STATIC3), topleft, 0);
    m_resizer.AddChild(GetDlgItem(IDC_STATIC4), topleft, RD_STRETCH_WIDTH);
    m_resizer.AddChild(GetDlgItem(IDC_STATIC5), topleft, 0);
    m_resizer.AddChild(GetDlgItem(IDC_STATIC6), topleft, 0);
    m_resizer.AddChild(GetDlgItem(IDC_STATIC7), topleft, 0);
    m_resizer.AddChild(GetDlgItem(IDC_STATIC8), topleft, 0);
    m_resizer.AddChild(m_imageCombo, topleft, RD_STRETCH_WIDTH);
    m_resizer.AddChild(m_materialCombo, topleft, RD_STRETCH_WIDTH);
    m_resizer.AddChild(m_ballDecalCombo, topleft, RD_STRETCH_WIDTH);
    m_resizer.AddChild(m_ballImageCombo, topleft, RD_STRETCH_WIDTH);
    m_resizer.AddChild(m_hReflectElementsCheck, topleft, 0);
    m_resizer.AddChild(m_reflectionStrengthEdit, topleft, RD_STRETCH_WIDTH);
    m_resizer.AddChild(m_hLogoModeCheck, topleft, 0);
    m_resizer.AddChild(m_ballReflectPlayfieldEdit, topleft, RD_STRETCH_WIDTH);
    m_resizer.AddChild(m_ballDefaultBulbIntensScaleEdit, topleft, RD_STRETCH_WIDTH);
    return TRUE;
}

INT_PTR TableVisualsProperty::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   m_resizer.HandleMessage(uMsg, wParam, lParam);
   return DialogProcDefault(uMsg, wParam, lParam);
}
