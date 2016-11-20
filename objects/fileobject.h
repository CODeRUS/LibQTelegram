#ifndef FILEOBJECT_H
#define FILEOBJECT_H

#include <QObject>
#include <QFile>
#include <QSize>
#include "../autogenerated/types/types.h"
#include "../mtproto/mtprotorequest.h"
#include "../mtproto/dc/dcsessionmanager.h"
#include "../cache/file/fileuploader.h"

class FileObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool downloading READ downloading NOTIFY downloadingChanged)
    Q_PROPERTY(bool downloaded READ downloaded NOTIFY downloadedChanged)
    Q_PROPERTY(bool hasThumbnail READ hasThumbnail NOTIFY hasThumbnailChanged)
    Q_PROPERTY(QSize imageSize READ imageSize NOTIFY imageSizeChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(QString filePath READ filePath NOTIFY filePathChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    Q_PROPERTY(TLInt fileSize READ fileSize NOTIFY fileSizeChanged)

    public:
        enum TransferMode {
            None = 0,
            Upload = 1,
            Download = 2,
            DownloadThumbnail = 3,
        };

    public:
        explicit FileObject(const QString& storagepath, QObject *parent = 0);
        explicit FileObject(FileUploader::MediaType mediatype, const QString& filepath, const QString &caption, const QString& storagepath, QObject *parent = 0);
        InputMedia* inputMedia();
        MessageMedia* messageMedia();
        FileUploader* uploader() const;
        Document* document() const;
        bool isUpload() const;
        bool downloading() const;
        bool downloaded() const;
        bool hasThumbnail() const;
        QSize imageSize() const;
        QString thumbnail() const;
        QString filePath() const;
        QString fileName() const;
        QString fileId() const;
        QString thumbnailId() const;
        TLInt fileSize() const;
        void setAutoDownload(bool autodownload);
        void setDocument(Document* document);
        void setFileSize(TLInt filesize);
        void setFileName(const QString& filename);
        void setImageSize(const QSize& imagesize);
        void setThumbnailLocation(FileLocation* filelocation);
        void setFileLocation(FileLocation* filelocation);
        void setFileId(const QString& fileid);
        void setThumbnailId(const QString& thumbnailid);
        void downloadThumbnail();
        bool loadCache();

    public slots:
        void download();
        void upload();

    private slots:
        void onUploadGetFileReplied(MTProtoReply* mtreply);
        void onUploaderCompleted();
        void sendDownloadRequest();

    private:
        void setDownloadMode(int downloadmode);
        void createDownloadSession(int dcid);
        void analyzeFile(const QString& filepath);

    signals:
        void fileSizeChanged();
        void imageSizeChanged();
        void thumbnailChanged();
        void filePathChanged();
        void fileNameChanged();
        void downloadedChanged();
        void downloadingChanged();
        void hasThumbnailChanged();
        void downloadCompleted();
        void uploadCompleted();


    private: // Uploads
        InputMedia* _inputmedia;
        MessageMedia* _messagemedia;

    private:
        QString _storagepath;
        bool _autodownload;
        int _transfermode;
        FileUploader* _fileuploader;
        Document* _document;
        FileLocation* _locthumbnail;
        FileLocation* _locfile;
        InputFileLocation* _inputfilelocation;
        DCSession* _dcsession;
        QFile* _file;
        QString _thumbnail;
        QString _filepath;
        QString _fileid;
        QString _thumbnailid;
        QString _filename;
        QSize _imagesize;
        TLInt _filesize;
};

#endif // FILEOBJECT_H
