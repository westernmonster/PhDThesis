package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.passes.matchers.AndMatcher;
import io.crowbar.instrumentation.passes.matchers.PrefixMatcher;
import io.crowbar.instrumentation.passes.matchers.OrMatcher;
import io.crowbar.instrumentation.passes.matchers.ReturnTypeMatcher;
import io.crowbar.instrumentation.passes.matchers.SuperclassMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;

import javassist.CtClass;
import javassist.CtMethod;

import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;


public final class JUnit3TestWrapper
extends AbstractTestWrapper {
    private static final Logger logger = LogManager.getLogger(JUnit3TestWrapper.class);

    private static final ActionTaker ACTION_TAKER =
        new WhiteList(
            new AndMatcher(
                new SuperclassMatcher("junit.framework.TestCase"),
                new ReturnTypeMatcher("void"),
                new OrMatcher(
                    new PrefixMatcher("test"),
                    new PrefixMatcher("run")
                    )));


    public static boolean isPass (Class cls,
                                  String methodName) {
        logger.debug("isPass({},{}) = {}",
                     cls.getName(),
                     methodName,
                     true);

        return true;
    }

    public static boolean isPass (Class cls,
                                  String methodName,
                                  Throwable e) {
        logger.debug("isPass({},{},{}) = {}",
                     cls.getName(),
                     methodName,
                     e.getClass().getName(),
                     false);
        return false;
    }

    @Override
    protected ActionTaker getActionTaker () {
        return ACTION_TAKER;
    }
}