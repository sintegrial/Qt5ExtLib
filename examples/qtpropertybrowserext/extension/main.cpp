#include <QApplication>
#include "QtVariantPropertyManagerExt.h"
#include "QtVariantEditorFactoryExt.h"
#include "qttreepropertybrowser.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QtVariantPropertyManagerExt *variantManager = new QtVariantPropertyManagerExt();

    QtProperty *topItem = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                QLatin1String("File Path Property"));

    QtVariantProperty *item1 = variantManager->addProperty(QVariant::Url, QLatin1String("Open File"));
    item1->setValue("autoexec.bat");
    item1->setAttribute("defaultPath", "C:\\");
    item1->setAttribute("filter", "Executables(*.exe;*.bat;*.com);;Text files(*.txt;*.doc);;Other files(*.*)");
    topItem->addSubProperty(item1);

    QtVariantProperty *item2 = variantManager->addProperty(QVariant::Url, QLatin1String("Save File"));
    item2->setValue("command.com");
    item2->setAttribute("defaultPath", "C:\\");
    item2->setAttribute("type", FPT_FILE_SAVE);
    topItem->addSubProperty(item2);

    QtVariantProperty *item3 = variantManager->addProperty(QVariant::Url, QLatin1String("Pick Directory"));
    item3->setValue("C:\\Windows");
    item3->setAttribute("type", FPT_DIRECTORY);
    topItem->addSubProperty(item3);

    QtVariantEditorFactoryExt *variantFactory = new QtVariantEditorFactoryExt();

    QtTreePropertyBrowser ed1;
    QtVariantPropertyManager *varMan = variantManager;
    ed1.setFactoryForManager(varMan, variantFactory);
    ed1.addProperty(topItem);

    ed1.resize(800,600);
    ed1.show();

    int ret = app.exec();

    delete variantFactory;
    delete variantManager;

    return ret;
}

