# Phase 2 L1T DQM plots

This needs to run within the Phase2 l1t-offline repo. Alternatively, if you have an EDM file that has all the required collections, that works too.

This produces a regular DQM file with all the subsystems stored in a single output.

To run, modify DQM/Phase2L1T/python/Phase2L1TSourceConfig_cff.py to run a specific module.

## Status update of each subsystem

CSC
Contact: Chad Freer (chadwfreer@gmail.com)
Status: Cannot find EMTFStage2Digis - the trigger primitives that hold the CSC digis. Not located in RelVals or after running the Phase 2 L1Sim sequence. Emailed Chad several times and still no answer.
Summary: Code runs but produces empty plots. Follow up with Chad further, or wait until digi producer is added to sequence.

DT
Contact: Javier Menendez (Javier.Fernandez@cern.ch)
Status: Runs - waiting for feedback.
Summary: Code runs (but only specially produced EDM file that contains the DT digis). Requested feedback on three occasions to know if quantities are sufficient. Still waiting.

ECal
Contact: David Petyt (david.anthony.petyt@cern.ch)
Status: Runs - waiting for feedback.
Summary: Code runs (but must first run the RawToDigi step again, and save the EcalRecHits). Awaiting feedback from David. Should also consider saving the Ecal RecHits in the SimL1 sequence.

GEM
Contact: Chad Freer (chadwfreer@gmail.com)
Status: There is no Phase2 data format yet. Code runs but only two empty plots are produced.
Summary: Waiting until data format and producer are created/added.

HCal
Contact: Georgia Karapostoli (Georgia.Karapostoli@cern.ch)
Status: Runs - waiting for feedback.
Summary: Code runs on RelVals alone (L1Sim step not required). I sent Georgia an example output file, and several reminders, and she has yet to get back to me.

HGCal
Contact: Jean-Baptiste Sauvan (Jean-Baptiste.Sauvan@cern.ch)
Status: Runs - feedback received. I believe this is final-ish.
Summary: Code runs on RelVals alone (L1Sim step not required). Jean-Baptiste says everything looks OK and he will update me if anything needs to be changed.

(i)RPC
Contact: Brieuc Francois (brieuc.francois@cern.ch)
Status: Runs - feedback received. I believe this is final-ish.
Summary: Code runs on RelVals, with the L1Sim step required. Brieuc says everything looks OK and he will update me if anything needs to be changed. However, iRPC is always empty.
