bool32 HandleWishPerishSongOnTurnEnd(void)
{
    gHitMarker |= (HITMARKER_GRUDGE | HITMARKER_IGNORE_BIDE);

    if ((gBattleTypeFlags & BATTLE_TYPE_ARENA)
     && gBattleStruct->arenaTurnCounter == 2
     && IsBattlerAlive(B_POSITION_PLAYER_LEFT) && IsBattlerAlive(B_POSITION_OPPONENT_LEFT))
    {
        s32 i;

        // This seems to be a bug?
        for (i = 0; i < 2; i++)
            CancelMultiTurnMoves(i);

        gBattlescriptCurrInstr = BattleScript_ArenaDoJudgment;
        BattleScriptExecute(BattleScript_ArenaDoJudgment);
        return TRUE;
    }

    gHitMarker &= ~(HITMARKER_GRUDGE | HITMARKER_IGNORE_BIDE);

    return FALSE;
}
