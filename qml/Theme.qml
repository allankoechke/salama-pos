import QtQuick 2.15
import QtQuick.Controls.Material 2.15

QtObject {
    id: theme
    
    // Material Design Color Palette
    readonly property color primary: "#12679a"  // Use explicit color instead of Material.accent
    readonly property color primaryVariant: Qt.darker(primary, 1.2)
    readonly property color secondary: "#03DAC6"
    readonly property color secondaryVariant: Qt.darker(secondary, 1.2)
    readonly property color background: Material.theme === Material.Dark ? "#121212" : "#FAFAFA"
    readonly property color surface: Material.theme === Material.Dark ? "#1E1E1E" : "#FFFFFF"
    readonly property color error: "#CF6679"
    readonly property color primaryText: "#FFFFFF"  // Text color on primary
    readonly property color secondaryText: "#000000"  // Text color on secondary
    readonly property color backgroundText: Material.theme === Material.Dark ? "#FFFFFF" : "#000000"  // Text color on background
    readonly property color surfaceText: Material.theme === Material.Dark ? "#FFFFFF" : "#000000"  // Text color on surface
    readonly property color errorText: "#FFFFFF"  // Text color on error
    
    // Custom app colors
    readonly property color menuColor: "#12679a"
    readonly property color tableHeaderColor: "#5f00ff"
    
    // Material Design elevation shadows
    readonly property int elevation0: 0
    readonly property int elevation1: 1
    readonly property int elevation2: 2
    readonly property int elevation4: 4
    readonly property int elevation6: 6
    readonly property int elevation8: 8
    readonly property int elevation12: 12
    readonly property int elevation16: 16
    readonly property int elevation24: 24
    
    // Typography
    readonly property int fontSizeH1: 96
    readonly property int fontSizeH2: 60
    readonly property int fontSizeH3: 48
    readonly property int fontSizeH4: 34
    readonly property int fontSizeH5: 24
    readonly property int fontSizeH6: 20
    readonly property int fontSizeSubtitle1: 16
    readonly property int fontSizeSubtitle2: 14
    readonly property int fontSizeBody1: 16
    readonly property int fontSizeBody2: 14
    readonly property int fontSizeButton: 14
    readonly property int fontSizeCaption: 12
    readonly property int fontSizeOverline: 10
    
    // Spacing (8dp grid)
    readonly property int spacing0: 0
    readonly property int spacing4: 4
    readonly property int spacing8: 8
    readonly property int spacing12: 12
    readonly property int spacing16: 16
    readonly property int spacing24: 24
    readonly property int spacing32: 32
    readonly property int spacing48: 48
    
    // Border radius
    readonly property int radiusSmall: 4
    readonly property int radiusMedium: 8
    readonly property int radiusLarge: 16
    
    // Material Design standard colors
    // Background colors
    function getBackgroundColor() {
        return Material.theme === Material.Dark ? "#121212" : "#FAFAFA"
    }
    
    // Surface colors (for cards, dialogs, etc.)
    function getSurfaceColor() {
        return Material.theme === Material.Dark ? "#1E1E1E" : "#FFFFFF"
    }
    
    // Text colors
    function getTextColor() {
        return Material.theme === Material.Dark ? Material.foreground : "#212121"
    }
    
    function getTextSecondaryColor() {
        return Material.theme === Material.Dark ? "#B0B0B0" : "#757575"
    }
    
    function getTextHintColor() {
        return Material.theme === Material.Dark ? "#666666" : "#9E9E9E"
    }
    
    // Divider colors
    function getDividerColor() {
        return Material.theme === Material.Dark ? "#3A3A3A" : "#E0E0E0"
    }
    
    // Foreground color (for Material components)
    function getForegroundColor() {
        return Material.theme === Material.Dark ? Material.foreground : "#212121"
    }
    
    // Update Material Design settings
    Component.onCompleted: {
        Material.accent = menuColor
        Material.primary = menuColor
    }
}

