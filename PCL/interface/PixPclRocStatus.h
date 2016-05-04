#ifndef PIXPCLROCSTATUS_h
#define PIXPCLROCSTATUS_h


// ----------------------------------------------------------------------
class PixPclRocStatus {
public:
  PixPclRocStatus();
  ~PixPclRocStatus();
  void fill(int idc);
  void fillOffline(int col, int row);
  int status(int idc);
  int status();

private:
  int fDC[26];

};

#endif
