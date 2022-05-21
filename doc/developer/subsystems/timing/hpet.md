# HPET driver

Theory of Operation
-------------------
At present, the HPET driver only makes use of the two HPET timers that are guaranteed by the specification to exist in the hardware HPET implementation, even if more are present.  Timer #0 is used for general requests; timer #1 is used for the periodic kernel tick (if timer #1 does not support the HPET periodic mode, it is reset upon every expiry so that the end result is effectively the same).

The process for establishing and processing a general timer request is as follows:

At present, the driver only supports requests in the form of delays (in nanoseconds) relative to the present.  The timing subsystem passes this to the driver in the form of a pointer to a `timing_request_t` structure with the relevant information.  The HPET driver thus first must convert the nanosecond delay into the units of time used by the monotonically-increasing HPET counter (such units are determined on driver and HPET initialization, in `hpet_init()` and subsidiary routines).  It then determines the current value of the HPET counter and adds the delay to that to determine the deadline upon which this timer will expire (note that the actual processing may take place some time later).  This is all done by `hpet_set_deadline_relative()`.

Next, the driver immediately and unconditionally inserts the request into a queue of HPET requests that is maintained internally by, and is only visible to, the driver.  This is accomplished by passing the request that was passed to the driver, as well as the calculated deadline, to `hpet_request_queue_add()`.  The request queue is currently implemented as a linked list and includes all outstanding timer requests in ascending order of expiry time; therefore, the insertion process requires finding the proper place in the queue in which to insert the request.

After inserting the request into the queue, control is passed to `hpet_add_oneshot()`.  This routine first ascertains whether the deadline of the newly-added request is sooner (numerically less) than the deadline currently entered into the comparator register of the HPET timer.  If and only if it is, `hpet_add_oneshot()` replaces the existing value in the comparator register with the deadline of the newly-added request; otherwise it does nothing further and returns.

Future features and improvements:
---------------------------------
* Set a timer to expire at a certain time rather than just a delay
* Use a more time-efficient method of maintaining the request queue