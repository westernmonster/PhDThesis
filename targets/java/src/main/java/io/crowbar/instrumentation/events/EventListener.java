package io.crowbar.instrumentation.events;

import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.instrumentation.runtime.Node;

public interface EventListener {
    void registerNode (Node n) throws Exception;

    void registerProbe (Probe p) throws Exception;

    void startTransaction (int probeId) throws Exception;

    void endTransaction (int probeId,
                         boolean[] hitVector) throws Exception;

    void oracle (int probeId,
                 double error,
                 double confidence) throws Exception;
}