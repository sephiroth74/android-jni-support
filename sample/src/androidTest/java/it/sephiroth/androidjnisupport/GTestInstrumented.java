package it.sephiroth.androidjnisupport;

import android.app.Activity;
import android.content.Intent;
import android.support.test.espresso.Espresso;
import android.support.test.rule.ActivityTestRule;
import android.support.test.runner.AndroidJUnit4;
import android.util.Log;
import android.widget.TextView;

import org.jetbrains.annotations.NotNull;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import it.sephiroth.androidjnisupport.condition_matcher.ConditionWatcher;
import it.sephiroth.androidjnisupport.condition_matcher.Instruction;

import static android.support.test.espresso.action.ViewActions.click;
import static android.support.test.espresso.assertion.ViewAssertions.matches;
import static android.support.test.espresso.matcher.ViewMatchers.withId;
import static android.support.test.espresso.matcher.ViewMatchers.withText;

@RunWith (AndroidJUnit4.class)
public class GTestInstrumented {
    public ActivityTestRule<MainActivity> activityRule = new ActivityTestRule<>(MainActivity.class, true, true);

    @Before
    public void setUp() {
        activityRule.launchActivity(new Intent());
    }

    @Test
    public void runAllTests() throws Exception {
        Espresso.onView(withId(R.id.button)).perform(click());
        ConditionWatcher.Companion.waitForCondition(new TextViewInstruction());
        Espresso.onView(withId(R.id.textView)).check(matches(withText("Result Code: 0")));
    }

    class TextViewInstruction extends Instruction {

        @NotNull
        @Override
        public String getDescription() {
            return "Result Code shouldn't be non empty";
        }

        @Override
        public boolean checkCondition() {
            Activity activity = activityRule.getActivity();
            TextView textView = activity.findViewById(R.id.textView);
            return textView.getText().length() > 0;
        }
    }
}
