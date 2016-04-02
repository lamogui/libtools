﻿/****************************************************************************
Nom ......... : midi.hpp
Role ........ : Déclare les classes ayant une relation avec le format MIDI
                voir : http://www.sonicspot.com/guide/midifiles.html
Auteur ...... : Kwon-Young CHOI & Julien DE LOOR
Version ..... : V1.7 olol
Licence ..... : © Copydown™
****************************************************************************/

#ifndef MIDIDECODER_HPP
#define MMIDIDECODER_HPP


#include <libtools/public/config.h>
#include <libtools/decoders/mididecoder.h>



class MidiHeader {
  public:

    MidiHeader();
    MidiHeader(uint16_t format, uint16_t tracks, uint16_t tick_per_beat);
    MidiHeader(uint16_t format, uint16_t tracks, uint8_t frame, uint8_t ticks);
    ~MidiHeader();
    
    bool read_from_buffer(const unsigned char* buffer, unsigned int size );
    
    bool write_to_buffer(unsigned char* buffer, unsigned int size ) const;
    bool write_to_file(FILE* file) const;
    static const unsigned size=14; //size in the final file in uint8_ts
    
    void print_infos() const ;
    
    inline float gain() const {if (!_gain) return 1.f; return _gain;}
    inline bool need_bpm() const { return _beat; }
    inline uint16_t format() const { return _format; }
    inline uint16_t tracks() const { return _tracks; }
    inline void set_tracks(uint16_t tracks) {  _tracks = tracks; }
    
  private:
    bool _beat;
    float _gain; //Represent the "precision tick gain" in relation to the signal refresh rate
                 //Valable pour le mode FPS
                 // Gain =  Midi Rate / Signal refreshRate
                 // Midi Rate = Signal refreshRate * Gain
                 // Midi Ticks = Signal Ticks * Gain
                 // Signal Ticks = Midi Ticks / Gain
                 //Valable pour le mode Beat
    uint16_t _format;
    uint16_t _tracks;
    uint16_t _division;
    
};

class MidiEvent {
  public:


  protected:

};

///MIDI Track 0 for format 1 ! 
class Midi_track0 {
  public:
    Midi_track0(); 
    Midi_track0(const std::string name); 
    ~Midi_track0(); 

    unsigned int size() const; //size in the final file in uint8_ts
    unsigned int read_from_buffer(const unsigned char* buffer, unsigned int size);
    
    bool write_to_buffer(unsigned char* buffer, unsigned int size ) const;
    bool write_to_file(FILE* file) const;
    
    void print_infos() const ;
    
    ///Getters / Setters
    inline void set_bpm(unsigned bpm) { if (bpm) _mpqn=60000000/bpm;}
    inline void set_music_name(const std::string n) {_music_name=n;}
    inline void set_copyright(const std::string c) {_copyright=c;}
    inline void set_comment(const std::string c) {_comment=c;}
    
    inline std::string music_name() { return _music_name; }
    inline std::string copyright() { return _copyright; }
    inline std::string comment() { return _comment; }
    inline unsigned int bpm() { if (_mpqn) return  60000000/_mpqn; return 0;}
    
    inline void reset() {
      _music_name.clear();
      _copyright.clear();
      _comment.clear();
      _mpqn=0;
    }
    
  private:
    unsigned int _mpqn;
    std::string _music_name;
    std::string _copyright;
    std::string _comment;
};

class Midi_track {
  public:
    Midi_track(MidiHeader& head);
    ~Midi_track();
    
    unsigned int size() const ; //size in the final file in uint8_ts
    inline unsigned int chunk_size() const {
      return _chunk_size;
    }
    
    inline const unsigned char* get_chunk() const {
      return _chunk;
    }
    
    unsigned int read_from_buffer(const unsigned char* buffer, unsigned int size);
    
    bool write_to_buffer(unsigned char* buffer, unsigned int size ) const;
    bool write_to_file(FILE* file) const;
    
    void push_midi_event(uint32_t midi_delta, uint8_t type, uint8_t chan, uint8_t p1, uint8_t p2);
    void push_midi_event(uint32_t midi_delta, uint8_t type, uint8_t chan, uint8_t p1);
    
    inline void reset() {_chunk_size=0;}
    
     inline const MidiHeader& get_head() const { return *_head; }
    
  private:
    void push_varlength(uint32_t var);
  
    inline void check_alloc(unsigned int wanted) 
    {
      if (_alloc_size < wanted+_chunk_size)
      {
        _alloc_size += wanted + 128;
        _chunk = (unsigned char*) realloc(_chunk,_alloc_size);
      }
    }
  
    MidiHeader* _head;
    unsigned char* _chunk;    //buffer for track data
    unsigned int _alloc_size; //Now allocated size
    uint32_t _chunk_size;        //Size of actual track datas
                              //Never include EndOfTrack
};



#define MIDI_META 0xFF
#define MIDI_TRACK_NAME 0x03
#define MIDI_SET_TEMPO 0x51
#define MIDI_TIME_SIGNATURE 0x58
#define MIDI_KEY_SIGNATURE 0x59
#define MIDI_END_OF_TRACK 0x2F
#define MIDI_PROGRAM_CHANGE 0xC
#define MIDI_CONTROLLER 0xB
#define MIDI_MAIN_VOLUME 0x07
#define MIDI_NOTE_ON 0x9
#define MIDI_NOTE_OFF 0x8

#include <libtools/core/file.hpp>

//// WARNING ZONE COMPLETE RECODING //// WARNING ZONE COMPLETE RECODING //// WARNING ZONE COMPLETE RECODING 
//// WARNING ZONE COMPLETE RECODING //// WARNING ZONE COMPLETE RECODING //// WARNING ZONE COMPLETE RECODING 

class MidiVar : public AbstractFileParser {
  public:
    MidiVar() : _var(0) {}
    MidiVar(unsigned int var) : _var(var) {}
    MidiVar(const MidiVar& m) : _var(m._var) {}
    ~MidiVar() {}

    virtual unsigned int read_from_buffer_offset(const unsigned char* buffer, 
                                                 unsigned int size,
                                                 unsigned int& offset)
    {
      const unsigned save_off=offset;
      _var=0;
      while (size > offset && buffer[offset] & 0x80)
        _var = (_var << 7) | (buffer[offset++] & 0x7F);
      if (size > offset)
        _var = buffer[offset++];
      return offset-save_off;
    }
    
    virtual unsigned int byte_size() const
    {
      unsigned int bit=0;
      for (uint32_t i=0; i<32; i++) {
        if ((_var >> bit)&1) bit=i;
      }
      return bit/7 + 1;
    }
    
    virtual unsigned int write_to_buffer_offset( unsigned char* buffer, 
                                                 unsigned int size,
                                                 unsigned int& offset) const 
    {
      const unsigned int parts = this->byte_size() - 1;
      const unsigned save_off=offset;
      if (size > offset + parts) {
        for (unsigned int i=0; i<parts; i++) 
            buffer[offset++] = ((_var>>(7*(parts-i))) & 0x7F) | 0x80;
        buffer[offset++] = _var & 0x7F;
      }
      return offset - save_off;                               
    }

    
    inline void set_var(uint32_t var) { _var=var;}
    inline uint32_t var() const { return _var;}
    
    inline operator uint32_t () { return _var;}
    inline MidiVar& operator=(const MidiVar& m) { _var=m._var; return *this;}
    inline unsigned int operator=(const unsigned int v) { _var=v; return v;}
    
  private:
    uint32_t _var;
};


class Midi_abstractevent : public AbstractFileParser{
  protected:
    Midi_abstractevent(MidiVar d, uint8_t type) : delta(d), _type(type) {}
    Midi_abstractevent( uint8_t type) : delta(0), _type(type) {}
  public:
    virtual ~Midi_abstractevent() {}

    inline uint8_t type() const { return ((_type & 0xF0)==0xF0) ? _type : _type >> 4; }
    
    //Return event validity
    //virtual bool is_valid() const = 0;
    //Return event size in uint8_ts
    virtual unsigned int byte_size() const =0;
    
    //Return writed size in uint8_ts WRITE THE WHOLE EVENT !
    virtual unsigned int write_to_buffer_offset( unsigned char* buffer, 
                                                 unsigned int size,
                                                 unsigned int& offset) 
                                                 const=0;
                                         
  static Midi_abstractevent* create_from_buffer(const unsigned char* buffer, 
                                                unsigned int buffer_size,
                                                unsigned int& offset);
                                              
  //Must return number of readed uint8_ts READ ONLY SPEC EVENT PARAMETERS (not delta and type)
  virtual unsigned int read_from_buffer_offset(const unsigned char* buffer, 
                                               unsigned int buffer_size,
                                               unsigned int& offset) = 0;
                                                 
  
  //Attributes
  public: MidiVar delta;
  protected: uint8_t _type;
};

class Midi_event : public Midi_abstractevent {
  public:
    Midi_event(MidiVar d,uint8_t type);
    Midi_event(uint8_t type, uint8_t channel, uint16_t p1 );
    Midi_event(uint8_t type, uint8_t channel, uint16_t p1 , uint16_t p2);
    virtual ~Midi_event();
    
    virtual unsigned int byte_size() const;
    virtual unsigned int write_to_buffer_offset( unsigned char* buffer, 
                                                 unsigned int size,
                                                 unsigned int& offset) const;
    virtual unsigned int read_from_buffer_offset(const unsigned char* buffer, 
                                                 unsigned int buffer_size,
                                                 unsigned int& offset);
    
    bool use_p2() const;
    inline uint8_t channel() const { return _type & 0xF; }
    
  //Attributes
  public:
    uint16_t p1;
    uint16_t p2;
};

class Midi_metaevent : public Midi_abstractevent {
  public:
    Midi_metaevent(MidiVar d,uint8_t meta_type);
    virtual ~Midi_metaevent();
    
    inline uint8_t meta_type() const { return _meta_type; }
    
    virtual unsigned int byte_size() const;
    
    virtual unsigned int write_meta_to_buffer( unsigned char* buffer, 
                                              unsigned int size,
                                              unsigned int& offset) const; //implement this !
    
    virtual unsigned int read_meta_from_buffer( const unsigned char* buffer, 
                                                unsigned int size,
                                                unsigned int& offset); //implement this !
    
    virtual unsigned int write_to_buffer_offset( unsigned char* buffer, 
                                                 unsigned int size,
                                                 unsigned int& offset) const;
    //offset must be after meta type !                                             
    virtual unsigned int read_from_buffer_offset(const unsigned char* buffer, 
                                                 unsigned int buffer_size,
                                                 unsigned int& offset);
    //Offset MUST BE AFTER 0xFF but before META TYPE !    
    static Midi_metaevent* create_from_buffer(const unsigned char* buffer, 
                                              unsigned int buffer_size,
                                              unsigned int& offset);
                                              
  protected:
    MidiVar _length;
    uint8_t _meta_type;
    
}; 


//// WARNING ZONE COMPLETE RECODING //// WARNING ZONE COMPLETE RECODING //// WARNING ZONE COMPLETE RECODING 
//// WARNING ZONE COMPLETE RECODING //// WARNING ZONE COMPLETE RECODING //// WARNING ZONE COMPLETE RECODING 

#endif
