#pragma once

#include <QTabWidget>
#include "MyWebEngineView.h"

class WebContain  : public QTabWidget
{
	Q_OBJECT

public:
	WebContain(QWidget *parent);
	~WebContain();
};

