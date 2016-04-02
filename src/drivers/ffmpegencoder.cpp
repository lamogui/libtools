
#include <libtools/drivers/ffmpegencoder.hpp>
#include <iostream>

#ifdef LIBTOOLS_WINDOWS
#include <windows.h>
#endif

FFmpegEncoder::FFmpegEncoder() :
  _encoding(false),
  _socketPort(4242)
{
}

FFmpegEncoder::~FFmpegEncoder()
{
  finnishEncoding();
}


bool FFmpegEncoder::startEncoding(const string_t &ffmpeg_exe,
    const string_t &output_filename,
    const string_t &input_cmd_line,
    const string_t &output_cmd_line)
{
  _encoding = false;

#ifdef SFML_TCPSOCKET_HPP
  _tcpListener.listen(_socketPort);
  _tcpListener.setBlocking(true);
#endif

  string_t cmd = string_t(L'"') + ffmpeg_exe + string_t(L'"');
  cmd += string_t(L" ") + input_cmd_line +  string_t(L" -i tcp://localhost:4242 ");
  cmd += output_cmd_line + string_t(L" -y ");
  cmd += string_t(L'"') + output_filename + string_t(L'"');

  std::cout << "FFmpeg cmd line: " << string_t_to_std(cmd) << std::endl;

#ifdef LIBTOOLS_WINDOWS
  if (WinExec(string_t_to_std(cmd).c_str(),SW_SHOW) > 31)
  {

  }
  else
  {
    return _encoding;
  }
#endif

#ifdef SFML_TCPSOCKET_HPP
 /* for (int i = 0 ; i < 6; i++)
  {*/
    if (_tcpListener.accept(_tcpSocket)==sf::Socket::Done)
    {
      _encoding = true;
     // break;
    }
    //sf::sleep(sf::milliseconds(1000));
  //}
#endif

  return _encoding;
}

uint64_t FFmpegEncoder::encodeData(const uint8_t *buffer, uint64_t size)
{
  if (_encoding)
  {
#ifdef SFML_TCPSOCKET_HPP
    uint64_t sent=0;
    std::size_t current_sent=0;
    sf::Socket::Status status;
    do
    {
      _tcpSocket.send((const void*) &(buffer[sent]),size,current_sent);
      size -= current_sent;
      sent += current_sent;

    } while (status==sf::Socket::Partial);

    if (status != sf::Socket::Done)
    {
      std::cerr << "premature end of encoding caused by socket problem (disconnect ?)" << std::endl;
      finnishEncoding();
    }

    return sent;
#endif
  }
  return 0;
}

void FFmpegEncoder::finnishEncoding()
{
#ifdef SFML_TCPSOCKET_HPP
  _tcpSocket.disconnect();
  _encoding=false;
#endif
}
