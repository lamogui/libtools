

#ifndef LIBTOOLS_FFMPEGENCODER_HPP
#define LIBTOOLS_FFMPEGENCODER_HPP

#include <libtools/public/config.h>

#ifdef SFML_TCPSOCKET_HPP
#include <SFML/Network.hpp>
#else
#error TODO Implement Qt/C++11 version of this

#endif

class FFmpegEncoder : public noncopyable_c
{
  public:
    FFmpegEncoder();
    virtual ~FFmpegEncoder();


    bool startEncoding(const string_t& ffmpeg_exe,
                       const string_t& output_filename,
                       const string_t& input_cmd_line ,
                       const string_t& output_cmd_line);

    uint64_t encodeData(const uint8_t* buffer, uint64_t size);

    void finnishEncoding();


    inline bool encoding() const
    {
      return _encoding;
    }

  protected:
    bool _encoding;
    uint16_t _socketPort;
#ifdef SFML_TCPSOCKET_HPP
    sf::TcpListener _tcpListener;
    sf::TcpSocket _tcpSocket;
#endif


};

#endif
