/* ============================================================
* QupZilla - WebKit based browser
* Copyright (C) 2010-2014  David Rosca <nowrep@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QIcon>
#include <QWebEngineView>

#include "qzcommon.h"
#include "loadrequest.h"

class WebPage;
class LoadRequest;
class IconLoader;
class WebHitTestResult;

class QUPZILLA_EXPORT WebView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit WebView(QWidget* parent = 0);
    ~WebView();

    QIcon icon() const;

    QString title() const;
    bool isTitleEmpty() const;

    WebPage* page() const;
    void setPage(WebPage* page);

    void load(const QUrl &url);
    void load(const LoadRequest &request);
    bool isLoading() const;

    int loadingProgress() const;
    void fakeLoadingProgress(int progress);

    // Set zoom level (0 - 17)
    int zoomLevel() const;
    void setZoomLevel(int level);

    void restoreHistory(const QByteArray &data);

    // Executes window.onbeforeunload, returns true if view can be closed
    bool onBeforeUnload();

    void addNotification(QWidget* notif);
    bool eventFilter(QObject *obj, QEvent *event);

    virtual QWidget* overlayWidget() = 0;

    static bool isUrlValid(const QUrl &url);
    static QUrl guessUrlFromString(const QString &string);
    static QList<int> zoomLevels();

    // Force context menu event to be sent on mouse release
    // This allows to override right mouse button events (eg. for mouse gestures)
    static bool forceContextMenuOnMouseRelease();
    static void setForceContextMenuOnMouseRelease(bool force);

signals:
    void iconChanged();
    void viewportResized(QSize);
    void showNotification(QWidget*);
    void privacyChanged(bool);
    void zoomLevelChanged(int);

public slots:
    void zoomIn();
    void zoomOut();
    void zoomReset();

    void editUndo();
    void editRedo();
    void editCut();
    void editCopy();
    void editPaste();
    void editSelectAll();
    void editDelete();

    void reloadBypassCache();

    void back();
    void forward();

    void showSource();
    void sendPageByMail();
    void savePageAs();

    void openUrlInNewTab(const QUrl &url, Qz::NewTabPositionFlags position);

    virtual void closeView() = 0;
    virtual void openNewTab(Qz::NewTabPositionFlags position) { Q_UNUSED(position) }
    virtual void loadInNewTab(const LoadRequest &req, Qz::NewTabPositionFlags position) = 0;

protected slots:
    void slotLoadStarted();
    void slotLoadProgress(int progress);
    void slotLoadFinished();
    void slotIconUrlChanged(const QUrl &url);

    // Context menu slots
    void openUrlInNewWindow();
    void sendLinkByMail();
    void copyLinkToClipboard();
    void downloadUrlToDisk();
    void openActionUrl();
    void showSiteInfo();
    void searchSelectedText();
    void searchSelectedTextInBackgroundTab();
    void bookmarkLink();
    void openUrlInSelectedTab();
    void openUrlInBackgroundTab();

    // To support user's option whether to open in selected or background tab
    void userDefinedOpenUrlInNewTab(const QUrl &url = QUrl(), bool invert = false);
    void userDefinedOpenUrlInBgTab(const QUrl &url = QUrl());

protected:
    void resizeEvent(QResizeEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

    virtual void _wheelEvent(QWheelEvent *event);
    virtual void _mousePressEvent(QMouseEvent *event);
    virtual void _mouseReleaseEvent(QMouseEvent *event);
    virtual void _mouseMoveEvent(QMouseEvent *event);
    virtual void _keyPressEvent(QKeyEvent *event);
    virtual void _keyReleaseEvent(QKeyEvent *event);
    virtual void _contextMenuEvent(QContextMenuEvent *event);

    void loadRequest(const LoadRequest &req);
    void applyZoom();

    void createContextMenu(QMenu *menu, const WebHitTestResult &hitTest);
    void createPageContextMenu(QMenu *menu);
    void createLinkContextMenu(QMenu *menu, const WebHitTestResult &hitTest);
    void createImageContextMenu(QMenu *menu, const WebHitTestResult &hitTest);
    void createSelectedTextContextMenu(QMenu *menu, const WebHitTestResult &hitTest);
    void createMediaContextMenu(QMenu *menu, const WebHitTestResult &hitTest);

    void checkForForm(QAction *action, const QPoint &pos);
    void createSearchEngine();


private slots:
    void addSpeedDial();
    void configureSpeedDial();
    void reloadAllSpeedDials();

    void toggleMediaPause();
    void toggleMediaMute();

private:
    void initializeActions();

    int m_currentZoomLevel;

    QIcon m_siteIcon;
    QUrl m_siteIconUrl;
    IconLoader* m_siteIconLoader;

    int m_progress;

    QUrl m_clickedUrl;
    QPoint m_clickedPos;

    WebPage* m_page;
    bool m_actionsInitialized;
    bool m_firstLoad;

    QObject *m_rwhvqt;

    static bool s_forceContextMenuOnMouseRelease;
};

#endif // WEBVIEW_H