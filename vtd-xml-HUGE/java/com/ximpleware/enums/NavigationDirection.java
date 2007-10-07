package com.ximpleware.enums;

/**
 * // Navigation directions
 * public final static int ROOT = 0;
 * public final static int PARENT = 1;
 * public final static int FIRST_CHILD = 2;
 * public final static int LAST_CHILD = 3;
 * public final static int NEXT_SIBLING = 4;
 * public final static int PREV_SIBLING = 5;
 * <p/>
 * // Navigation directions
 * public final static int R = 0;
 * public final static int P = 1;
 * public final static int FC = 2;
 * public final static int LC = 3;
 * public final static int NS = 4;
 * public final static int PS = 5;
 */
public enum NavigationDirection
{
    ROOT,
    PARENT,
    FIRST_CHILD,
    LAST_CHILD,
    NEXT_SIBLING,
    PREV_SIBLING;

    // Shorthand
    public final static NavigationDirection R = ROOT;
    public final static NavigationDirection P = PARENT;
    public final static NavigationDirection FC = FIRST_CHILD;
    public final static NavigationDirection LC = LAST_CHILD;
    public final static NavigationDirection NS = NEXT_SIBLING;
    public final static NavigationDirection PS = PREV_SIBLING;


}
