
// Precompiled headers
#include "Stable.h"

#include "AppearanceDockWidget.h"
#include "RenderThread.h"

QAppearanceWidget::QAppearanceWidget(QWidget* pParent) :
	QWidget(pParent),
	m_MainLayout(),
	m_TransferFunctionWidget(),
	m_NodePropertiesWidget(),
	m_PresetsWidget(NULL, "Appearance", "Appearance")
{
	// Create main layout
	m_MainLayout.setAlignment(Qt::AlignTop);
	setLayout(&m_MainLayout);

	m_MainLayout.addWidget(&m_TransferFunctionWidget, 1, 0);
	m_MainLayout.addWidget(&m_NodePropertiesWidget, 2, 0);
	m_MainLayout.addWidget(&m_PresetsWidget, 0, 0);

	QObject::connect(&m_PresetsWidget, SIGNAL(LoadPreset(const QString&)), this, SLOT(OnLoadPreset(const QString&)));
	QObject::connect(&gRenderStatus, SIGNAL(LoadPreset(const QString&)), &m_PresetsWidget, SLOT(OnLoadPreset(const QString&)));
	QObject::connect(&m_PresetsWidget, SIGNAL(SavePreset(const QString&)), this, SLOT(OnSavePreset(const QString&)));
}

void QAppearanceWidget::OnLoadPreset(const QString& Name)
{
	// Only load the preset when it exists
	if (!m_PresetsWidget.HasPreset(Name))
		return;

	QTransferFunction TransferFunction = m_PresetsWidget.GetPreset(Name);

	
	if (Scene())
	{
		TransferFunction.SetRangeMin(Scene()->m_IntensityRange.m_Min);
		TransferFunction.SetRangeMax(Scene()->m_IntensityRange.m_Max);
	}

	gTransferFunction = TransferFunction;

	// De-normalize node intensity
// 	TransferFunction.DeNormalizeIntensity();
}

void QAppearanceWidget::OnSavePreset(const QString& Name)
{
	QTransferFunction Preset = gTransferFunction;
	Preset.SetName(Name);

	// Save the preset
	m_PresetsWidget.SavePreset(Preset);
}

QAppearanceDockWidget::QAppearanceDockWidget(QWidget *parent) :
	QDockWidget(parent),
	m_VolumeAppearanceWidget()
{
	setWindowTitle("Appearance");
	setToolTip("<img src=':/Images/palette.png'><div>Volume Appearance</div>");
	setWindowIcon(GetIcon("palette"));

	m_VolumeAppearanceWidget.setParent(this);

	setWidget(&m_VolumeAppearanceWidget);
}