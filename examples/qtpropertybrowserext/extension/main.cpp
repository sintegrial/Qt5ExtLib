#include <QApplication>
#include <QTime>
#include <QMap>
#include <QPair>

#include "QtVariantPropertyManagerExt.h"
#include "QtVariantEditorFactoryExt.h"
#include "qttreepropertybrowser.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setStyle("fusion");

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

    QtProperty *topItem2 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                QLatin1String("QVector3D Property"));

    QtVariantProperty *itemV3d = variantManager->addProperty(QVariant::Vector3D, QLatin1String("Default QVector3D"));
    itemV3d->setValue(QVector3D(10,20,30));
    // itemV3d->setAttribute("minimum", QVector3D(-10,-10,-10));
    //itemV3d->setAttribute("maximum", QVector3D(40,40,40));
    topItem2->addSubProperty(itemV3d);

    QtVariantProperty *itemV3d2 = variantManager->addProperty(QVariant::Vector3D, QLatin1String("Limited QVector3D (-1...1)"));
    itemV3d2->setAttribute("minimum", QVector3D(-1,-1,-1));
    itemV3d2->setAttribute("maximum", QVector3D(1,1,1));
    itemV3d2->setAttribute("decimals", 1);
    itemV3d2->setAttribute("singleStep", 0.1f);
    itemV3d2->setValue(QVector3D(0.1f, -0.1f, 0.0f));
    topItem2->addSubProperty(itemV3d2);

    QtProperty *topItem3 = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                QLatin1String("Extended Properties"));

    QtVariantProperty *itemBool = variantManager->addProperty(QVariant::Bool, QLatin1String("Checkbox only"));
    itemBool->setAttribute("textVisible", false);
    topItem3->addSubProperty(itemBool);

    QtVariantProperty *itemInt = variantManager->addProperty(QVariant::Int, QLatin1String("Integer with suffix"));
    itemInt->setAttribute("suffix", "%");
    itemInt->setValue(50);
    itemInt->setAttribute("minimum", "0");
    itemInt->setAttribute("maximum", "250");
    TIntStringList intPresets;
    intPresets << TIntStringPair(10, "10%") << TIntStringPair(20, "20%") << TIntStringPair(30, "30%");
    itemInt->setAttribute("preset", QVariant::fromValue(intPresets));
    topItem3->addSubProperty(itemInt);

    QtVariantProperty *itemFloat = variantManager->addProperty(QVariant::Double, QLatin1String("Real with suffix"));
    itemFloat->setAttribute("suffix", "USD");
    itemFloat->setValue(1230.99);
    topItem3->addSubProperty(itemFloat);

    QtVariantProperty *itemFloat1 = variantManager->addProperty(QVariant::Double, QLatin1String("Real with prefix"));
    itemFloat1->setAttribute("prefix", "$");
    itemFloat1->setValue(1230.99); 
    TDoubleStringList doublePresets;
    doublePresets << TDoubleStringPair(100, "$100") << TDoubleStringPair(200, "$200") << TDoubleStringPair(1000, "$1000")
                      << TDoubleStringPair(2000, "$2000") << TDoubleStringPair(100000, "$100000");
    itemFloat1->setAttribute("preset", QVariant::fromValue(doublePresets));
    topItem3->addSubProperty(itemFloat1);

    QtVariantProperty *itemTime = variantManager->addProperty(QVariant::Time, QLatin1String("Time with prefix & suffix"));
    itemTime->setAttribute("prefix", "After");
    itemTime->setAttribute("suffix", "o'clock");
    itemTime->setValue(QTime::currentTime());
    topItem3->addSubProperty(itemTime);


    QtVariantEditorFactoryExt *variantFactory = new QtVariantEditorFactoryExt();

    QtTreePropertyBrowser ed1;
    QtVariantPropertyManager *varMan = variantManager;
    ed1.setFactoryForManager(varMan, variantFactory);

    ed1.addProperty(topItem);
    ed1.addProperty(topItem2);
    ed1.addProperty(topItem3);

    ed1.resize(800,600);
    ed1.show();

    int ret = app.exec();

    delete variantFactory;
    delete variantManager;

    return ret;
}

