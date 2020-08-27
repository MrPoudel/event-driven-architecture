#ifndef OBSERVER_HPP
#define OBSERVER_HPP

// This is 
class ObseverMsg {
  public:
     virtual ~ObseverMsg() {}
};

template<typename PayloadType>
class ObseverDataMsg: public ObseverMsg {
  public:
    ObseverDataMsg(){}
    ObseverDataMsg(PayloadType* pType): pl_(pType) {}
    virtual ~ObseverDataMsg() {}

    PayloadType& getPayload() const
    {
    return *pl_;
    }

  private:
    PayloadType* pl_;
};
/**
 * Interface for the Observer
 */
class Observer {

public:

    /**
     * Update the state of this observer
     * @param temp new temperaure
     * @param humidity new humidity
     * @param pressure new pressure
     */
    virtual void update(ObseverMsg* data) = 0;

};

#endif