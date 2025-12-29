import QtQuick 2.15
import QtQuick.Controls.Material 2.15

Text {
    property real size: 15
    property bool useMaterialColors: true
    
    font.family: montserratFontLoader.name
    font.pixelSize: size
    
    // Use Material Design colors when enabled
    color: useMaterialColors ? (Material.theme === Material.Dark ? Material.foreground : "#212121") : color
}
