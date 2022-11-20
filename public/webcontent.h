Search for code or files
chromium/src
main
headless/public/headless_web_contents.h

headless

app

lib

public

domains

internal

util
headless_browser.cc
headless_browser.h
headless_browser_context.h
headless_devtools_channel.h
headless_devtools_client.h
headless_devtools_target.h
headless_export.h
headless_shell.h
headless_web_contents.h
version.h.in

test
BUILD.gn
COMMON_METADATA
DEPS
DIR_METADATA
OWNERS
README.md
headless.gni
protocol_config.json
headless_web_contents.h
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
88
89
90
91
92
93
94
95
96
97
98
99
100
101
102
103
104
105
106
107
108
109
110
111
112
113
114
115
116
117
118
119
120
121
122
123
124
125
126
127
128
129
130
131
132
133
134
135
136
137
138
139
140
141
142
143
144
145
// Copyright 2015 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef HEADLESS_PUBLIC_HEADLESS_WEB_CONTENTS_H_
#define HEADLESS_PUBLIC_HEADLESS_WEB_CONTENTS_H_

#include <string>
#include <utility>

#include "base/callback.h"
#include "base/memory/raw_ptr.h"
#include "base/process/kill.h"
#include "headless/public/headless_devtools_channel.h"
#include "headless/public/headless_export.h"
#include "ui/gfx/geometry/size.h"
#include "url/gurl.h"

namespace headless {
class HeadlessBrowserContextImpl;
class HeadlessBrowserImpl;
class HeadlessDevToolsTarget;

// Class representing contents of a browser tab. Should be accessed from browser
// main thread.
class HEADLESS_EXPORT HeadlessWebContents {
 public:
  class HEADLESS_EXPORT Builder;

  HeadlessWebContents(const HeadlessWebContents&) = delete;
  HeadlessWebContents& operator=(const HeadlessWebContents&) = delete;

  virtual ~HeadlessWebContents() {}

  class HEADLESS_EXPORT Observer {
   public:
    Observer(const Observer&) = delete;
    Observer& operator=(const Observer&) = delete;

    // All the following notifications will be called on browser main thread.

    // Indicates that this HeadlessWebContents instance is now ready to be
    // inspected using a HeadlessDevToolsClient.
    //
    // TODO(altimin): Support this event for pages that aren't created by us.
    virtual void DevToolsTargetReady() {}

    // Indicates that a DevTools client attached to this HeadlessWebContents
    // instance.
    virtual void DevToolsClientAttached() {}

    // Indicates that a DevTools client detached from this HeadlessWebContents
    // instance.
    virtual void DevToolsClientDetached() {}

    // This method is invoked when the process of the observed RenderProcessHost
    // exits (either normally or with a crash). To determine if the process
    // closed normally or crashed, examine the |status| parameter.
    //
    // If |status| is TERMINATION_STATUS_LAUNCH_FAILED then |exit_code| will
    // contain a platform specific launch failure error code. Otherwise, it will
    // contain the exit code for the process.
    virtual void RenderProcessExited(base::TerminationStatus status,
                                     int exit_code) {}

    // Invoked when HeadlessWebContents is being destroyed.
    virtual void HeadlessWebContentsDestroyed() {}

   protected:
    Observer() {}
    virtual ~Observer() {}
  };

  // Add or remove an observer to receive events from this WebContents.
  // |observer| must outlive this class or be removed prior to being destroyed.
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  // Return a DevTools target corresponding to this tab. Note that this method
  // won't return a valid value until Observer::DevToolsTargetReady has been
  // signaled.
  virtual HeadlessDevToolsTarget* GetDevToolsTarget() = 0;

  // Creates a DevTools channel corresponding to this tab. Note that this method
  // won't return a valid value until Observer::DevToolsTargetReady has been
  // signaled.
  virtual std::unique_ptr<HeadlessDevToolsChannel> CreateDevToolsChannel() = 0;

  // Close this page. |HeadlessWebContents| object will be destroyed.
  virtual void Close() = 0;

  // Returns the main frame's process id or -1 if there's no main frame.
  virtual int GetMainFrameRenderProcessId() const = 0;

  // Returns the main frame's node id or -1 if there's no main frame.
  virtual int GetMainFrameTreeNodeId() const = 0;

  // Returns the main frame's devtools id or "" if there's no main frame.
  virtual std::string GetMainFrameDevToolsId() const = 0;

 protected:
  HeadlessWebContents() {}
};

class HEADLESS_EXPORT HeadlessWebContents::Builder {
 public:
  Builder(const Builder&) = delete;
  Builder& operator=(const Builder&) = delete;

  ~Builder();
  Builder(Builder&&);

  // Set an initial URL to ensure that the renderer gets initialized and
  // eventually becomes ready to be inspected. See
  // HeadlessWebContents::Observer::DevToolsTargetReady. The default URL is
  // about:blank.
  Builder& SetInitialURL(const GURL& initial_url);

  // Specify the initial window size (default is configured in browser options).
  Builder& SetWindowSize(const gfx::Size& size);

  // Specify whether BeginFrames should be controlled via DevTools commands.
  Builder& SetEnableBeginFrameControl(bool enable_begin_frame_control);

  // The returned object is owned by HeadlessBrowser. Call
  // HeadlessWebContents::Close() to dispose it.
  HeadlessWebContents* Build();

 private:
  friend class HeadlessBrowserImpl;
  friend class HeadlessBrowserContextImpl;
  friend class HeadlessWebContentsImpl;

  explicit Builder(HeadlessBrowserContextImpl* browser_context);

  raw_ptr<HeadlessBrowserContextImpl, DanglingUntriaged> browser_context_;

  GURL initial_url_ = GURL("about:blank");
  gfx::Size window_size_;
  bool enable_begin_frame_control_ = false;
};

}  // namespace headless

#endif  // HEADLESS_PUBLIC_HEADLESS_WEB_CONTENTS_H_
