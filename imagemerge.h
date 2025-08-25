#ifndef IMAGEMERGE_H
#define IMAGEMERGE_H

#include <QImage>

class ImageMerge
{
public:
    ImageMerge();
    ImageMerge(const QImage &image1, const QImage &image2);
    void setImages(int index, const QImage &image);
    int merge(QImage &outputImage);
    void setOffset(int offset);
    int offset() const { return m_offset; }

    static int mergeImages(const QImage &image1, const QImage &image2, int offset, QImage &outputImage);
    static int exposeImage(const QImage &image, int value, QImage &outputImage);

private:
    QImage m_img1;
    QImage m_img2;
    int m_offset = 30;
};

#endif // IMAGEMERGE_H
