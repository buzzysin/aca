#ifndef MAKES_TIMESTAMP_H
#define MAKES_TIMESTAMP_H

class MakesTimestamp {
public:
  virtual ~MakesTimestamp() {}

public:
  unsigned long now() const;
};

#endif