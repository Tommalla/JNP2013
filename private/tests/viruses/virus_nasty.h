#ifndef VIRUS_NASTY_H
#define VIRUS_NASTY_H
#include "../exceptional.h"

using namespace std;

template<class ID, unsigned int prob>
class VirusNasty {
  public:
    typedef ID id_type;
    VirusNasty(id_type const &_id) : 
        id(_id)
    {
        Exceptional<prob>::mess();
    }
    id_type get_id() {
      Exceptional<prob>::mess();
      return id;
    }
  private:
    id_type id;
};

#endif // VIRUS_NASTY_H
