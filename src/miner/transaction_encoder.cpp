#include "transaction_encoder.h"
#include "data/problem.h"

int TransactionEncoder::OperationPairToItem(OperationId prev_oid, OperationId oid) const
{
  // encode OriginOp in JobId 0 and other ops in JobId+1
  JobId prev_jid = (prev_oid + ref_pb.nMac - 1) / ref_pb.nMac;

  // create a unique item number for a pair of successive operations
  // each operation can have a predecessor from the nJob or the origin job
  return static_cast<int>(oid * (ref_pb.nJob + 1) + prev_jid);
}

std::pair<OperationId, OperationId> TransactionEncoder::ItemToOperationPair(int itid) const
{
  OperationId oid = static_cast<unsigned>(itid) / (ref_pb.nJob + 1);
  JobId jid = (oid - 1) / ref_pb.nMac;
  JobId prev_jid = static_cast<unsigned>(itid) % (ref_pb.nJob + 1);

  OperationId prev_oid;
  if (prev_jid == 0)
  {
    prev_oid = ref_pb.OriginOp;
  }
  else
  {
    prev_jid -= 1;
    if (prev_jid == jid)
    {
      prev_oid = ref_pb.prevOperation[oid];
    }
    else
    {
      MachineId mid = ref_pb.machineNumber[oid];
      prev_oid = ref_pb.operationsOnMachine[mid][prev_jid];
    }
  }

  return {prev_oid, oid};
}
