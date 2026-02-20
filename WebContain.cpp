#include "stdafx.h"
#include "WebContain.h"

WebContain::WebContain(QWidget *parent)
	: QTabWidget(parent)
{
	this->setTabsClosable(true);
	connect(this, &QTabWidget::tabCloseRequested, this, &QTabWidget::removeTab);
	connect(this, &QTabWidget::tabCloseRequested, this, &WebContain::updateWebList);
}

WebContain::~WebContain()
{
}

void WebContain::updateWebList(int index) {
	webList.removeAt(index);
}

