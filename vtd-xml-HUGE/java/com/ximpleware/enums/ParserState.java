package com.ximpleware.enums;

/**
 * Created by IntelliJ IDEA.
 * User: Duane
 * Date: Aug 12, 2007
 * Time: 12:28:41 AM
 * To change this template use File | Settings | File Templates.
 */
public enum ParserState
{
    STATE_LT_SEEN, // encounter the first
    STATE_START_TAG,
    STATE_END_TAG,
    STATE_ATTR_NAME,
    STATE_ATTR_VAL,
    STATE_TEXT,
    STATE_DOC_START,    // beginning of document
    STATE_DOC_END,       // end of document
    STATE_PI_TAG,
    STATE_PI_VAL,
    STATE_DEC_ATTR_NAME,
    STATE_COMMENT,
    STATE_CDATA,
    STATE_DOCTYPE,
    STATE_END_COMMENT,
    STATE_END_PI
    // STATE_END_PI_VAL

}
