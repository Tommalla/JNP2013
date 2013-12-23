#ifndef VIRUS_NORMAL_H
#define VIRUS_NORMAL_H

template<class ID>
class VirusNormal {
  public:
    typedef ID id_type;
    VirusNormal(id_type const &_id) : id(_id) {
    }
    id_type get_id() const {
      return id;
    }
  private:
    id_type id;
};

#endif // VIRUS_NORMAL_H
