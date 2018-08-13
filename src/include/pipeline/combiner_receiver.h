/*-------------------------------------------------------------------------
 *
 * combiner_receiver.h
 *	  An implementation of DestReceiver that that allows combiners to receive
 *	  tuples from worker processes.
 *
 * Copyright (c) 2013-2015, PipelineDB
 *
 * IDENTIFICATION
 *	  src/include/pipeline/combiner_receiver.h
 *
 */
#ifndef COMBINER_RECEIVER_H
#define COMBINER_RECEIVER_H

#include "tcop/dest.h"
#include "pipeline/executor.h"

typedef bool (*CombinerReceiveFunc) (ContQuery *query, uint32 shard_hash, uint64 group_hash, HeapTuple tup);
extern CombinerReceiveFunc CombinerReceiveHook;
typedef void (*CombinerFlushFunc) (void);
extern CombinerFlushFunc CombinerFlushHook;

typedef struct CombinerSyncFollower CombinerSyncFollower;

/*
 * Interface that allows extensions to subscribe to the output of combiner syncs
 */
typedef struct CombinerSyncFollower
{
	void (*begin) (CombinerSyncFollower *c);
	void (*commit) (CombinerSyncFollower *c);
	void (*abort) (CombinerSyncFollower *c);
	void (*insert) (CombinerSyncFollower *c, Relation rel, HeapTuple tup);
	void (*update) (CombinerSyncFollower *c, Relation rel, HeapTuple otup, HeapTuple ntup);
	void *state;
} CombinerSyncFollower;

typedef  CombinerSyncFollower *(*CreateCombinerSyncFollowerFunc) (ContQuery *cq);
extern CreateCombinerSyncFollowerFunc CreateCombinerSyncFollowerHook;

extern DestReceiver *CreateCombinerDestReceiver(void);
extern void SetCombinerDestReceiverParams(DestReceiver *self, ContExecutor *cont_exec, ContQuery *query);
extern void SetCombinerDestReceiverHashFunc(DestReceiver *self, FuncExpr *hash);
extern void CombinerDestReceiverFlush(DestReceiver *self);

#endif
